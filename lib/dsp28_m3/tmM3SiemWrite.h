#ifndef	D__TMM3SIEMWRITE_V01
#define	D__TMM3SIEMWRITE_V01

#include	"../../tmprotocol.h"
#include	"../dsp28/tmTaskDsp28.h"


class	CtmM3SiemWrite:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmM3SiemWrite)
	public:
		CtmM3SiemWrite();
		~CtmM3SiemWrite();
	
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


#ifndef	D__TMM3SIEMWRITE_V01
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
