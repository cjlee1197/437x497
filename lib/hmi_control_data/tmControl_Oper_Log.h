/*===========================================================================+
|  Class    : CtmControl_Error		                                         |
|  Task     : Alarm Server Function 				    	         		 |
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
#ifndef	D_CTMCONTROL_OPER_LOG
#define	D_CTMCONTROL_OPER_LOG
#include	"../../tmcontrol.h"
#include	"../../tmpackclass.h"
#include	"../../package.h"

class	CtmControl_Oper_Log: public	CtmControl
{
	DECLARE_DYNCREATE(CtmControl_Oper_Log)
	public:
		CtmControl_Oper_Log();
		~CtmControl_Oper_Log();
	public:
		BOOL    		AddOperLog(DWORD DBIndex , DWORD OldValue, DWORD NewValue);
		BOOL    		ClearOperLog();
		void*				GetOperLog(int nIndex);
		WORD     		GetOperLogCount();
		void				Init_OperLog();			
	private:
		pthread_mutex_t					m_MutexOperLogMsg;				//¦h½uµ{Âê
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
};

#ifndef	D_BEOBJECT_CTMCONTROL_OPER_LOG	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

