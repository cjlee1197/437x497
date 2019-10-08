#ifndef	D__TMDSP28ACTUALVALUE_V01
#define	D__TMDSP28ACTUALVALUE_V01

#include	"../../tmprotocol.h"
#include	"tmTaskPicker.h"


class	CtmPickerActualValue:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmPickerActualValue)
	public:
		CtmPickerActualValue();
		~CtmPickerActualValue();
	
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

#ifndef	D_BEOBJECT_CTMPICKERACTUALVALUE
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
