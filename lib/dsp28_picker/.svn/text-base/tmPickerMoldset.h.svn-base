#ifndef	D__TMPICKERMOLDSET_V01
#define	D__TMPICKERMOLDSET_V01

#include	"../../tmprotocol.h"
#include	"tmTaskPicker.h"


class	CtmPickerMoldset:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmPickerMoldset)
	public:
		CtmPickerMoldset();
		~CtmPickerMoldset();
	
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



#ifndef	D_BEOBJECT_CTMPICKERMOLDSET
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
