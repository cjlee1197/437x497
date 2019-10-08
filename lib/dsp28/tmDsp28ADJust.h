#ifndef	D__TMDSP28ADJUST_V01
#define	D__TMDSP28ADJUST_V01

#include	"../../tmprotocol.h"


class	CtmDsp28ADJust:  public CtmProtocol
{
	DECLARE_DYNCREATE(CtmDsp28ADJust)
	public:
		CtmDsp28ADJust();
		~CtmDsp28ADJust();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		

		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMDSP28ADJUST
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
