#ifndef	D__TMPICKERCOMMAND_V01
#define	D__TMPICKERCOMMAND_V01

#include	"../../tmprotocol.h"
#include	"tmTaskPicker.h"


class	CtmPickerCommand:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmPickerCommand)
	public:
		CtmPickerCommand();
		~CtmPickerCommand();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		
		
		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMPICKERCOMMAND	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
