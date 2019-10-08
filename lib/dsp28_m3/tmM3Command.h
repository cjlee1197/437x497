#ifndef	D__TMM3COMMAND_V01
#define	D__TMM3COMMAND_V01

#include	"../../tmprotocol.h"
#include	"../dsp28/tmTaskDsp28.h"


class	CtmM3Command:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmM3Command)
	public:
		CtmM3Command();
		~CtmM3Command();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		
		
		//	override 
		int		CreateSelf()	;	//	overwrite
	
	
	
};

#ifndef	D_BEOBJECT_CTMM3COMMAND	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
