#ifndef		D__PACKEDCLASS
#define		D__PACKEDCLASS
#include	"CObject.h"

const	int	CNT_INT_REGFUNC_OVERRIDE		= -2;			//	overload
const	int	CNT_INT_REGFUNC_INVALID			= -1;			//	novaild
const	int	CNT_INT_REGFUNC_RETURN_NOREG	= 0x7FFFFFFF;


/*!	\class CtmPackClass 
*	\brief 所有包的祖類.
*	
*	\author fans.
*	\note	提供類的一些基本方法
*	
*/
class CtmPackClass: public CObject
{
	DECLARE_DYNCREATE(CtmPackClass)	
	
	public:
		CtmPackClass();
		~CtmPackClass();
	public:
		typedef	int	(CtmPackClass::*pRegFunc)	(int nParaNum, void* pParas[]);
		typedef	struct	TagRegFunc
		{
			char		szFuncName[256];
			pRegFunc	pFunc;
			TagRegFunc*	pNext;
		}	tmRegFunc;
	public:
		virtual	int					CreateSelf();
		virtual	int					FreeSelf();
		virtual	CtmPackClass*		CloneSelf()		{return NULL;};
		virtual	int					RegFunc(char* pszFuncName, pRegFunc pFunc);
		virtual	int					ExeRegFunc(char* pszFuncName, int nParaNum, void* pParas[]);
	protected:
		virtual	void				FreeRegFunc(tmRegFunc* pRegs);
	
	protected:
		tmRegFunc*			m_pRegFunc;
};





#endif
