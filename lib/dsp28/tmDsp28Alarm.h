#ifndef	D__TMDSP28ALARM_V01
#define	D__TMDSP28ALARM_V01

#include	"../../tmprotocol.h"
#include	"tmTaskDsp28.h"


class	CtmDsp28Alarm:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmDsp28Alarm)
	public:
		CtmDsp28Alarm();
		~CtmDsp28Alarm();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		
		//int		GetType			();
		//int		RevType			();
		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMDSP28ALARM
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
