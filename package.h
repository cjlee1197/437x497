
#ifndef		D_CTMPACKAGS
#define		D_CTMPACKAGS
//#include 	<fstream.h>
#include	"inifile.h"
#include	"CObject.h"


//using namespace std;


typedef	CObject*	(*pFuncCreateInstance)(char* pszName);
typedef	void		(*pFuncReleaseInstance)(CObject* pInstance);

/*!	\struct tagtmPackageInfo
 *	\typedef typedef struct tagtmPackageInfo tmPackageInfo
 *	\brief
 *
 */
typedef struct tagtmPackageInfo
{
	/*!
		\var	char	tagtmPackageInfo::szPackageName[256]
	*	\brief	包的名稱，就是so的名稱.
	*	\note	需要注意的是：其so的名稱與包中實現類的名稱應該符合以下標準：C+PackageName-.so=類名.
	*/
	
	/*!
		\var	char	tagtmPackageInfo::szFunctionName[256]
	*	\brief	包的名稱功能，標記著這個包的主要功能.
	*/
	
	/*!
		\var	void*	tagtmPackageInfo::pHandle
	*	\brief	so的句柄.
	*/
	
	/*!
		\var	pFuncCreateInstance	tagtmPackageInfo::pCreate.
	*	\brief	so中的CreateInstance函數指針.
	*/
	
	/*!
		\var	pFuncReleaseInstance	tagtmPackageInfo::pRelease.
	*	\brief	so中的ReleaseInstance函數指針.
	*/
	char					szPackageName[256];			
	char					szFunctionName[256];
	void*					pHandle;
	pFuncCreateInstance		pCreate;
	pFuncReleaseInstance 	pRelease;
}	tmPackageInfo;


/*!	\class CtmHmiPackage 
*	\brief 管理所有的包.
*	不僅僅是管理了所有的包，其對外提供的接口是創建包中類的實例
*	\author fans.
*	\note	所有的包中必須要有CreateInstance()和ReleaseInstance()方法
*	
*/
class CtmHmiPackage
{
	protected:
		CtmHmiPackage();
		~CtmHmiPackage();
	public:
		static	CtmHmiPackage*	CreateInstance();
		static	void			ReleaseInstance();
	public:
		//CObject* 	CreateClassInstance(char* pszName);
		CObject*	CreateClassInstance(char* pszName, char* pszName1 = NULL);
		void		ReleaseClassInstance(char* pszName, CObject* pInstance);
	protected:
		int			LoadPackage();
		int 		UnLoadPackage();
		
		void		ParseClassName(char* pszInput, char* pszOut);	
	
	protected:
		static		CtmHmiPackage*	m_pInstance;
	
	private:
		tmPackageInfo*	m_pPackages;	
		CIniFile		m_IniPackage;
		int				m_nIniHandle;
		int				m_nPackageNum;
		char*			m_pPackageKind[1024];
		//fstream m_IniFile;
		 
		
};

extern	CtmHmiPackage*		g_pPackage;


#endif
