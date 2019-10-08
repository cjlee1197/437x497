#ifndef	D__TMM3MOLDSETSTATUS_V01
#define	D__TMM3MOLDSETSTATUS_V01

#include	"../../tmprotocol.h"
#include	"../dsp28/tmTaskDsp28.h"


class	CtmM3MoldsetStatus:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmM3MoldsetStatus)
	public:
		CtmM3MoldsetStatus();
		~CtmM3MoldsetStatus();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		
		
		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMM3MOLDSETSTATUS	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
