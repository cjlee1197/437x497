/*===========================================================================+
|  Class    : CtmControlServer_lib	                                         |
|  Task     : All the muster Control Service				    	         |
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
#ifndef	D_CTMCONTROLSERVER_LIB
#define	D_CTMCONTROLSERVER_LIB
#include	"../../tmcontrol.h"
#include	"../../tmpackclass.h"
#include	"../../package.h"

class	CtmControlServer_lib: public	CtmControlServer
{
	DECLARE_DYNCREATE(CtmControlServer_lib)
	public:
		CtmControlServer_lib();
		~CtmControlServer_lib();
		
	public:
		/*virtual*/CtmControl*	ErrorControl()			{ return m_pControl[ERRORINDEX];};
		/*virtual*/CtmControl*	MoniControl()			{ return m_pControl[MONIINDEX];};
		/*virtual*/CtmControl*	MoldControl()			{ return m_pControl[MOLDINDEX];};
		/*virtual*/CtmControl*	QualityControl()		{ return m_pControl[QUALITYINDEX];};
		/*virtual*/CtmControl*	UserControl()			{ return m_pControl[USERINDEX];};
		/*virtual*/CtmControl*	OtherControl(int nIndex){ return m_pControl[nIndex];};
		/*virtual*/CtmControl*	OtherControl(char* pName){ /*return m_pControl[nIndex];*/};
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
};	

#ifndef	D_BEOBJECT_CTMCONTROLSERVER_LIB	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

