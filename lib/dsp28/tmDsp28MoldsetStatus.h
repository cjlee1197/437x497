#ifndef	D__TMDSP28MOLDSETSTATUS_V01
#define	D__TMDSP28MOLDSETSTATUS_V01

#include	"../../tmprotocol.h"
#include	"tmTaskDsp28.h"


class	CtmDsp28MoldsetStatus:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmDsp28MoldsetStatus)
	public:
		CtmDsp28MoldsetStatus();
		~CtmDsp28MoldsetStatus();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		//int		ParseProtocol	(void* pInData,		int nInputLen);		
		
		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMDSP28MOLDSETSTATUS	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
