/*===========================================================================+
|  Class    : CtmControl_Monitor		                                     |
|  Task     : Moni Service Function 				    	         		 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James	                                                         |
|  Version  : V1.00                                                          |
|  Creation : 05/11/2008	                                                 |
|  Revision : 05/11/2008				                                     |
+===========================================================================*/
#ifndef	D_CTMCONTROL_MONITOR
#define	D_CTMCONTROL_MONITOR
#include	"../../tmcontrol.h"
#include	"../../tmpackclass.h"
#include	"../../package.h"
#include	"../../tmcontrol.h"
#include	"../libControl.h"

class	CtmControl_Monitor: public	CtmControl
{
	DECLARE_DYNCREATE(CtmControl_Monitor)
	public:
		CtmControl_Monitor();
		~CtmControl_Monitor();
		
	public:
		/*virtual*/BOOL			AddMoni2()								;
		/*virtual*/BOOL			AddMoni2_COMMON()								;
		/*virtual*/BOOL			AddMoni2_1HMI2DSP()								;
		/*virtual*/BOOL    		ClearMoniLog()							;
		/*virtual*/void			Destroy_MoniLog()						;
		/*virtual*/void*		GetMoniIndex()							;
		/*virtual*/void*		GetMoniLog(int nIndex)					;
		/*virtual*/WORD        	GetMoniLogCount()						;
		/*virtual*/void			Init_MoniLog()							;
		/*virtual*/BOOL			SaveMoni2(I32_MONI2* pI32Moni2)			;
		/*virtual*/int			_GetMoniStructSize()					;
	private:
		pthread_mutex_t					m_MutexMoni;				//多線程鎖
		static		tmDB_MONI2			m_MoniCurrent;				//當前監測
		static		tmDB_MONI2			m_MoniIndex;				//記錄當前最新的個數和序號
		static		int					m_nShotCount;
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
};	

#ifndef	D_BEOBJECT_CTMCONTROL_MONITOR	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

