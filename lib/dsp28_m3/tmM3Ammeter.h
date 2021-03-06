#ifndef	D__TMM3AMMETER_V01
#define	D__TMM3AMMETER_V01

#include	"../../tmprotocol.h"
#include	"../dsp28/tmTaskDsp28.h"


class	CtmM3Ammeter:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmM3Ammeter)
	public:
		CtmM3Ammeter();
		~CtmM3Ammeter();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		
		//	override 
		int		CreateSelf()	;	//	overwrite
		int		FreeSelf();			//	overwrite
	private:
		//CtmProtocol* m_pCommonRead;
		
	
	
	
};

#ifndef	D_BEOBJECT_CTMM3AMMEMTER
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
