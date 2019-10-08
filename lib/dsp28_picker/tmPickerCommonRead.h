#ifndef	D__TMPICKERCOMMONREAD_V01
#define	D__TMPICKERCOMMONREAD_V01

#include	"../../tmprotocol.h"
#include	"tmTaskPicker.h"


class	CtmPickerCommonRead:  public CtmDsp28Protocol
{
	DECLARE_DYNCREATE(CtmPickerCommonRead)
	public:
		CtmPickerCommonRead();
		~CtmPickerCommonRead();
	
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


class	CtmPickerCommonDiagRead: public CtmPickerCommonRead
{
	DECLARE_DYNCREATE(CtmPickerCommonDiagRead)
	public:
		CtmPickerCommonDiagRead();
		~CtmPickerCommonDiagRead();
	public:
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		SetDataToDataBase(void* pData, int nCount, BOOL bSend);
		
		//	override 
		int		CreateSelf()	;	//	overwrite
	protected:
		int		m_nAryDiagValues[256];
};


#ifndef	D_BEOBJECT_CTMPICKERCOMMONREAD
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
