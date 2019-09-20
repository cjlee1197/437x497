
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
	*	\brief	�]���W�١A�N�Oso���W��.
	*	\note	�ݭn�`�N���O�G��so���W�ٻP�]����{�����W�����ӲŦX�H�U�зǡGC+PackageName-.so=���W.
	*/
	
	/*!
		\var	char	tagtmPackageInfo::szFunctionName[256]
	*	\brief	�]���W�٥\��A�аO�۳o�ӥ]���D�n�\��.
	*/
	
	/*!
		\var	void*	tagtmPackageInfo::pHandle
	*	\brief	so���y�`.
	*/
	
	/*!
		\var	pFuncCreateInstance	tagtmPackageInfo::pCreate.
	*	\brief	so����CreateInstance��ƫ��w.
	*/
	
	/*!
		\var	pFuncReleaseInstance	tagtmPackageInfo::pRelease.
	*	\brief	so����ReleaseInstance��ƫ��w.
	*/
	char					szPackageName[256];			
	char					szFunctionName[256];
	void*					pHandle;
	pFuncCreateInstance		pCreate;
	pFuncReleaseInstance 	pRelease;
}	tmPackageInfo;


/*!	\class CtmHmiPackage 
*	\brief �޲z�Ҧ����].
*	���ȶȬO�޲z�F�Ҧ����]�A���~���Ѫ����f�O�Ыإ]���������
*	\author fans.
*	\note	�Ҧ����]�������n��CreateInstance()�MReleaseInstance()��k
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
