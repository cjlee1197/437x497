#ifndef	D__TMM3SIEMREAD_V01
#define	D__TMM3SIEMREAD_V01

#include	"../../tmprotocol.h"
#include	"../dsp28/tmTaskDsp28.h"


class	CtmM3SiemRead:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmM3SiemRead)
	public:
		CtmM3SiemRead();
		~CtmM3SiemRead();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		
		int		SetDataToDataBase(void* pData, int nCount, BOOL bSend);
		//	override 
		int		CreateSelf()	;	//	overwrite
	protected:
		BYTE		m_nAryDiagValues[256];
};

#ifndef	D_BEOBJECT_CTMM3SIEMREAD
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
