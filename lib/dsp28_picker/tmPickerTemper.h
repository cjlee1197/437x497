#ifndef	D__TMPICKERTEMPER_V01
#define	D__TMPICKERTEMPER_V01

#include	"../../tmprotocol.h"


class	CtmPickerTemper:  public CtmProtocol
{
	DECLARE_DYNCREATE(CtmPickerTemper)
	public:
		CtmPickerTemper();
		~CtmPickerTemper();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		

		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMPICKERTEMPER
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
