#ifndef	D__TMDSP28ACTUALVALUE_V01
#define	D__TMDSP28ACTUALVALUE_V01

#include	"../../tmprotocol.h"
#include	"tmTaskDsp28.h"


class	CtmDsp28ActualValue:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmDsp28ActualValue)
	public:
		CtmDsp28ActualValue();
		~CtmDsp28ActualValue();
	
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

#ifndef	D_BEOBJECT_CTMDSP28ACTUALVALUE
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
