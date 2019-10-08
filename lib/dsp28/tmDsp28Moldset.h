#ifndef	D__TMDSP28MOLDSET_V01
#define	D__TMDSP28MOLDSET_V01

#include	"../../tmprotocol.h"
#include	"tmTaskDsp28.h"


class	CtmDsp28Moldset:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmDsp28Moldset)
	public:
		CtmDsp28Moldset();
		~CtmDsp28Moldset();
	
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



#ifndef	D_BEOBJECT_CTMDSP28MOLDSET
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
