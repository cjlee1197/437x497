#ifndef	D__TMDSP28LINESTATUS_V01
#define	D__TMDSP28LINESTATUS_V01

#include	"../../tmprotocol.h"
#include	"tmTaskDsp28.h"


class	CtmDsp28LineStatus:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmDsp28LineStatus)
	public:
		CtmDsp28LineStatus();
		~CtmDsp28LineStatus();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		//int		ParseProtocol	(void* pInData,		int nInputLen);		
		
		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMDSP28LINESTATUS
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
