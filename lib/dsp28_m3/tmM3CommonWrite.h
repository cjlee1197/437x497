#ifndef	D__TMM3COMMONWRITE_V01
#define	D__TMM3COMMONWRITE_V01

#include	"../../tmprotocol.h"
#include	"../dsp28/tmTaskDsp28.h"


class	CtmM3CommonWrite:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmM3CommonWrite)
	public:
		CtmM3CommonWrite();
		~CtmM3CommonWrite();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		
		int		SetDataToDataBase(void* pData, int nCount, BOOL bSend);
		//int		GetType			();
		//int		RevType			();
		//override 
		int		CreateSelf()	;	//	overwrite
};



class	CtmM3CommonDiagWrite: public CtmM3CommonWrite
{
	DECLARE_DYNCREATE(CtmM3CommonDiagWrite)
	public:
		CtmM3CommonDiagWrite();
		~CtmM3CommonDiagWrite();
	public:
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		//	override 
		int		CreateSelf()	;	//	overwrite
};

#ifndef	D_BEOBJECT_CTMM3COMMONWRITE
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
