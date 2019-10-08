#ifndef	D__TMPICKERADJUST_V01
#define	D__TMPICKERADJUST_V01

#include	"../../tmprotocol.h"


class	CtmPickerADJust:  public CtmProtocol
{
	DECLARE_DYNCREATE(CtmPickerADJust)
	public:
		CtmPickerADJust();
		~CtmPickerADJust();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		

		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMPICKERADJUST
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
