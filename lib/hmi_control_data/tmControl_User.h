/*===========================================================================+
|  Class    : CtmControl_User			                                     |
|  Task     : User Service Function 				    	         		 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James	                                                         |
|  Version  : V1.00                                                          |
|  Creation : 05/11/2008	                                                 |
|  Revision : 05/11/2008				                                     |
+===========================================================================*/
#ifndef	D_CTMCONTROL_USER
#define	D_CTMCONTROL_USER
#include	"../../tmcontrol.h"
#include	"../../tmpackclass.h"
#include	"../../package.h"
#include	"../../tmcontrol.h"

class	CtmControl_User: public	CtmControl
{
	DECLARE_DYNCREATE(CtmControl_User)
	public:
		CtmControl_User();
		~CtmControl_User();
		
	public:
		/*virtual*/BOOL    	AddUser(char* pszID, char* pszPassword, DWORD dwPrivilege)	;
		/*virtual*/BOOL    	DeleteUser(char* pszID)										;
		/*virtual*/short   	FindFreeUser()												;
		/*virtual*/short		FindUser(char* pszID)										;
		/*virtual*/void			InitCurrentUser()											;
		/*virtual*/WORD    	LoginLocal(char* pszID, char* pszPassword)	;
		/*virtual*/WORD    	GetPrivilefe(char* pszID)	;
		/*virtual*/BOOL    	ModifyUser(char* pszID, char* pszPassword, DWORD dwPrivilege);
		/*virtual*/BOOL			SaveCurrentUser(char* pszName, WORD wPrivilege)				;
		/*virtual*/WORD     Encode(char* psz)											;
		/*virtual*/void   	IniUser();								;
		/*virtual*/WORD   	GetUserNum();
	private:
		pthread_mutex_t		m_MutexUser;				//¦h½uµ{Âê
		WORD 	userindex;
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
};	

#ifndef	D_BEOBJECT_CTMCONTROL_USER	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

