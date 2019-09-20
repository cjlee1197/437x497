#include	"package.h"
#include	"tmdebug.h"



const 	char*	CNT_STR_SECTION_NAME	= "lib";
const 	char*	CNT_STR_PACKAGE_CREATE	= "CreateInstance";
const 	char*	CNT_STR_PACKAGE_RELEASE = "ReleaseInstance";


CtmHmiPackage* CtmHmiPackage::m_pInstance = NULL;
CtmHmiPackage*		g_pPackage = NULL;//CtmHmiPackage::CreateInstance();	
/*!	\fn CtmHmiPackage	CtmHmiPackage::CreateInstance()
 *	\brief	�����������ҫ��w�A�����L�k�q�L��L���~�|�h������.
 *		��������ҫ��w�Ȧ�1��
 *	\return ��^���O��������ҫ��w
 *	\note	�O�ҥ�������ҶȦ��@��
*/
CtmHmiPackage*	CtmHmiPackage::CreateInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new CtmHmiPackage;
	return m_pInstance;
}


/*!	\fn void	CtmHmiPackage::ReleaseInstance()
 *	\brief	�����������
 *	\note	�b�@�몺���p�U�A���ݭn����������ҡA�Ȧb�h�X���ɭԤ~�|�ե�.
*/
void	CtmHmiPackage::ReleaseInstance()
{
	delete m_pInstance;
}

/*!	\fn CtmHmiPackage::CtmHmiPackage()
 *	\brief	�u�����c�y���.
 *	\note	�ȶȳQ�����ϥ�
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
 *	\brief	�u�����R�y���.
 *	\note	�ȶȳQ�����ϥ�
*/
CtmHmiPackage::~CtmHmiPackage()
{
	UnLoadPackage();
	m_IniPackage.CloseFile();
}


/*!	\fn int	CtmHmiPackage::LoadPackage()
 *	\brief	�˸��].
 *	�����O�˸��]�A���۱N�]������Ө�ƤޥΥX�ӡGCreateInstance�MReleaseInstance.
 *	\return �˸��X�k���]��
 *	\note	�ݭn�`�N���O�˸����]�Ƥ��i��O�L����A�����@�@�i�H�˸�1024�ӥ]
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
			pszError = dlerror();	//	�ݭn�˴����~
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
 *	\brief	�����].
 *	�䥻��N�O�N�Ҧ���library����.
 *	\return �����]���ѥ�
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
 *	\brief	�ھڵ��w���\��W�ٳЫ�������ҡ]�����O��{�Ӷ��\�઺�^.
 *	�ھڥ\��W�ٳЫت����A�@����{���bso��.
 *	\param	[in]	pszName	�\�઺�W�١A�bconfig���N�O=��������
 *	\param	[i]		pszName1 �l�\��ﶵ
 *	\return �ЫإX������Ҫ����w
 *	\note	�ݭn�`�N�Ҧ��i�Ыت����A�䥲���OCObject��������
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
 *	\brief	�ھڵ��w���\��W������������ҡ]�����O��{�Ӷ��\�઺�^.
 *	�PCreateClassInstance�t��ϥΡA�ھڥ\��W������������ҡA�@����{���bso��.
 *	\param	[in]	pszName	�\�઺�W�١A�bconfig���N�O=��������
 *	\param	[in]	pInstance �ݭn���񪺹�ҫ��w.
 *	\note	�ݭn�`�N�Ҧ��������A�䥲���OCObject��������
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
 *	\brief	�ھ�so���W�ٸѪR�X���W.
 *	\param	[in]	pszInput	so���W�١]library���W�١^
 *	\param	[out]	pszOut �ѪR�X�Ӫ������W��
 *	\note	�ݭn�`�N���O�G��so���W�ٻP�]����{�����W�����ӲŦX�H�U�зǡGC+PackageName-.so=���W.
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


