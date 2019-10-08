#ifndef	D__TMDSP28COMMONREAD_V01
#define	D__TMDSP28COMMONREAD_V01

#include	"../../tmprotocol.h"
#include	"tmTaskDsp28.h"


class	CtmDsp28CommonRead:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmDsp28CommonRead)
	public:
		CtmDsp28CommonRead();
		~CtmDsp28CommonRead();
	
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


class	CtmDsp28CommonDiagRead: public CtmDsp28CommonRead
{
	DECLARE_DYNCREATE(CtmDsp28CommonDiagRead)
	public:
		CtmDsp28CommonDiagRead();
		~CtmDsp28CommonDiagRead();
	public:
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		SetDataToDataBase(void* pData, int nCount, BOOL bSend);
		
		//	override 
		int		CreateSelf()	;	//	overwrite
	protected:
		int		m_nAryDiagValues[256];
};


#ifndef	D_BEOBJECT_CTMDSP28COMMONREAD
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
