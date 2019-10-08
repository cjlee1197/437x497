#ifndef	D__TMM3TEMPER_V01
#define	D__TMM3TEMPER_V01

#include	"../../tmprotocol.h"


class	CtmM3Temper:  public CtmProtocol
{
	DECLARE_DYNCREATE(CtmM3Temper)
	public:
		CtmM3Temper();
		~CtmM3Temper();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		

		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMM3TEMPER
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
