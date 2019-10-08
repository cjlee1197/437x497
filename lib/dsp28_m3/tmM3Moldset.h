#ifndef	D__TMM3MOLDSET_V01
#define	D__TMM3MOLDSET_V01

#include	"../../tmprotocol.h"
#include	"../dsp28/tmTaskDsp28.h"


class	CtmM3Moldset:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmM3Moldset)
	public:
		CtmM3Moldset();
		~CtmM3Moldset();
	
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



#ifndef	D_BEOBJECT_CTMM3MOLDSET
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
