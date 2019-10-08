#ifndef	D__TMDSP28COMMONWRITE_V01
#define	D__TMDSP28COMMONWRITE_V01

#include	"../../tmprotocol.h"
#include	"tmTaskDsp28.h"


class	CtmDsp28CommonWrite:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmDsp28CommonWrite)
	public:
		CtmDsp28CommonWrite();
		~CtmDsp28CommonWrite();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		
		//int		GetType			();
		//int		RevType			();
		//override 
		int		CreateSelf()	;	//	overwrite
};



class	CtmDsp28CommonDiagWrite: public CtmDsp28CommonWrite
{
	DECLARE_DYNCREATE(CtmDsp28CommonDiagWrite)
	public:
		CtmDsp28CommonDiagWrite();
		~CtmDsp28CommonDiagWrite();
	public:
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		//	override 
		int		CreateSelf()	;	//	overwrite
};

#ifndef	D_BEOBJECT_CTMDSP28COMMONWRITE
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
