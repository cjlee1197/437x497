#ifndef	D__TMM3COMMONREAD_V01
#define	D__TMM3COMMONREAD_V01

#include	"../../tmprotocol.h"
#include	"../dsp28/tmTaskDsp28.h"


class	CtmM3CommonRead:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmM3CommonRead)
	public:
		CtmM3CommonRead();
		~CtmM3CommonRead();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		
		int		SetDataToDataBase(void* pData, int nCount, BOOL bSend);
		//	override 
		int		CreateSelf()	;	//	overwrite
};


class	CtmM3CommonDiagRead: public CtmM3CommonRead
{
	DECLARE_DYNCREATE(CtmM3CommonDiagRead)
	public:
		CtmM3CommonDiagRead();
		~CtmM3CommonDiagRead();
	public:
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		SetDataToDataBase(void* pData, int nCount, BOOL bSend);
		
		//	override 
		int		CreateSelf()	;	//	overwrite
	protected:
		int		m_nAryDiagValues[256];
};


#ifndef	D_BEOBJECT_CTMM3COMMONREAD
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
