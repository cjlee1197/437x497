#ifndef	D__TMPICKERCOMMONWRITE_V01
#define	D__TMPICKERCOMMONWRITE_V01

#include	"../../tmprotocol.h"
#include	"tmTaskPicker.h"


class	CtmPickerCommonWrite:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmPickerCommonWrite)
	public:
		CtmPickerCommonWrite();
		~CtmPickerCommonWrite();
	
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



class	CtmPickerCommonDiagWrite: public CtmPickerCommonWrite
{
	DECLARE_DYNCREATE(CtmPickerCommonDiagWrite)
	public:
		CtmPickerCommonDiagWrite();
		~CtmPickerCommonDiagWrite();
	public:
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		//	override 
		int		CreateSelf()	;	//	overwrite
};

#ifndef	D_BEOBJECT_CTMPICKERCOMMONWRITE
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
