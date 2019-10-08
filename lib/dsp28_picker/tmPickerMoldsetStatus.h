#ifndef	D__TMPICKERMOLDSETSTATUS_V01
#define	D__TMPICKERMOLDSETSTATUS_V01

#include	"../../tmprotocol.h"
#include	"tmTaskPicker.h"


class	CtmPickerMoldsetStatus:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmPickerMoldsetStatus)
	public:
		CtmPickerMoldsetStatus();
		~CtmPickerMoldsetStatus();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		//int		ParseProtocol	(void* pInData,		int nInputLen);		
		
		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMPICKERMOLDSETSTATUS	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
