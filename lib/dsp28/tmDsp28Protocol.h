#ifndef 	D__TMDSP28PROTOCOL_V01
#define		D__TMDSP28PROTOCOL_V01
#include	"../../tmprotocol.h"



class	CtmDsp28Protocol: public CtmHostProtocol
{
	
	DECLARE_DYNCREATE(CtmDsp28Protocol)
	public:
		CtmDsp28Protocol();
		~CtmDsp28Protocol();
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		
	private:
		CtmProtocol*	m_pTemp[32];
};


#ifndef	D_BEOBJECT_CTMDSP28PROTOCOL	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif
#endif
