#ifndef 	D__TMPICKERPROTOCOL_V01
#define		D__TMPICKERPROTOCOL_V01
#include	"../../tmprotocol.h"



class	CtmPickerProtocol: public CtmHostProtocol
{
	
	DECLARE_DYNCREATE(CtmPickerProtocol)
	public:
		CtmPickerProtocol();
		~CtmPickerProtocol();
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		
	private:
		CtmProtocol*	m_pTemp[32];
};


#ifndef	D_BEOBJECT_CTMPICKERPROTOCOL	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif
#endif
