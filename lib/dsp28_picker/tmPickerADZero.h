#ifndef	D__TMPICKERADZERO_V01
#define	D__TMPICKERADZERO_V01

#include	"../../tmprotocol.h"


class	CtmPickerADZero:  public CtmProtocol
{
	DECLARE_DYNCREATE(CtmPickerADZero)
	public:
		CtmPickerADZero();
		~CtmPickerADZero();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		

		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMPICKERADZERO
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
