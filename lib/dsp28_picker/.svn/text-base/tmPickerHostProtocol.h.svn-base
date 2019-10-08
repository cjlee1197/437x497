#ifndef 	D__TMPICKERHOSTPROTOCOL_V01
#define		D__TMPICKERHOSTPROTOCOL_V01
#include	"../../tmprotocol.h"



class	CtmPickerHostProtocol: public CtmHostProtocol
{
	
	DECLARE_DYNCREATE(CtmPickerHostProtocol)
	public:
		CtmPickerHostProtocol();
		~CtmPickerHostProtocol();
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		int		Compose(int nType, 	void* pOutData, void* pInData, int nInputLen, void* pEx = NULL);
		int		Parse(void* pInData,int nInputLen);
		CtmProtocol*	GetSendProtocol(int nType);
		CtmProtocol*	GetRevProtocol(int nType);
		int 	GetType(void* pInData, int nInputLen);
	private:
		CtmProtocol*	m_pTemp[32];
};


#ifndef	D_BEOBJECT_CTMPICKERHOSTPROTOCOL	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif
#endif
