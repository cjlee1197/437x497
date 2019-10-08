#ifndef 	D__TMDSP54HOSTPROTOCOL_V01
#define		D__TMDSP54HOSTPROTOCOL_V01
#include	"../../tmprotocol.h"

class	CtmDsp54HostProtocol: public CtmHostProtocol
{
	
	DECLARE_DYNCREATE(CtmDsp54HostProtocol)
	public:
		CtmDsp54HostProtocol();
		~CtmDsp54HostProtocol();
	public:
	    int		GetType(void* pInData, int nInputLen);
	    int		Compose(int nType, 	void* pOutData, void* pInData, int nInputLen, void* pEx = NULL);
		int		Parse(void* pInData,int nInputLen);
		CtmProtocol*	GetSendProtocol(int nType);
		CtmProtocol*	GetRevProtocol(int nType);
		int		CreateSelf();
		int		FreeSelf();
	private:
		CtmProtocol*	m_pTemp[32];
};


#ifndef	D_BEOBJECT_CTMDSP54HOSTPROTOCOL
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif
#endif
