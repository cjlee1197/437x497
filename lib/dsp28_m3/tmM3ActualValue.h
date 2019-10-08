#ifndef	D__TMM3ACTUALVALUE_V01
#define	D__TMM3ACTUALVALUE_V01

#include	"../../tmprotocol.h"
#include	"../dsp28/tmTaskDsp28.h"


class	CtmM3ActualValue:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmM3ActualValue)
	public:
		CtmM3ActualValue();
		~CtmM3ActualValue();
	
	public:
		//================================================================//
		//	override CtmProtocol;
		//================================================================//
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);		
		int		SetDataToDataBase(void* pData, int nCount, BOOL bSend);
		//	override 
		int		CreateSelf()	;	//	overwrite
		int		FreeSelf();			//	overwrite
	private:
		//CtmProtocol* m_pCommonRead;
		
	
	
	
};

#ifndef	D_BEOBJECT_CTMM3ACTUALVALUE
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
