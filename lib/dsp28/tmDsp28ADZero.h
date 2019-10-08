#ifndef	D__TMDSP28ADZERO_V01
#define	D__TMDSP28ADZERO_V01

#include	"../../tmprotocol.h"


class	CtmDsp28ADZero:  public CtmProtocol
{
	DECLARE_DYNCREATE(CtmDsp28ADZero)
	public:
		CtmDsp28ADZero();
		~CtmDsp28ADZero();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		

		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMDSP28ADZERO
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
