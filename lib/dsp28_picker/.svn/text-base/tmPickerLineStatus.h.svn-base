#ifndef	D__TMPICKERLINESTATUS_V01
#define	D__TMPICKERLINESTATUS_V01

#include	"../../tmprotocol.h"
#include	"tmTaskPicker.h"


class	CtmPickerLineStatus:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmPickerLineStatus)
	public:
		CtmPickerLineStatus();
		~CtmPickerLineStatus();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		//int		ParseProtocol	(void* pInData,		int nInputLen);		
		
		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMPickerLINESTATUS
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
