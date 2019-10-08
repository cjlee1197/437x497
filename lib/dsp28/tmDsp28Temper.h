#ifndef	D__TMDSP28TEMPER_V01
#define	D__TMDSP28TEMPER_V01

#include	"../../tmprotocol.h"


class	CtmDsp28Temper:  public CtmProtocol
{
	DECLARE_DYNCREATE(CtmDsp28Temper)
	public:
		CtmDsp28Temper();
		~CtmDsp28Temper();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		

		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMDSP28TEMPER
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
