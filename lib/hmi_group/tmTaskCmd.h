#ifndef		D__TMTASKCMD28_V01
#define		D__TMTASKCMD28_V01
#include	"../../taskcmd.h"




class  CtmTaskCmd: public CTaskCmd
{
	friend	void _SaveScreen(CtmTaskCmd *pTaskCmd);
	DECLARE_DYNCREATE(CtmTaskCmd)
	public:
		CtmTaskCmd();
		~CtmTaskCmd();
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		pthread_t	m_ScanManualThread;
		void	WndProc(int message, WPARAM wParam, LPARAM lParam); 
		//====================
		//	override the function that are defined in the CTask
		//====================
		void    Run();
		
	private:
    	int 			m_nSaveScreenFlag;
    	pthread_t 		m_SaveScreenThreadID;
	
};

void 	ManualRun(WORD	wCode);
void 	_SaveScreen(CtmTaskCmd *pTaskCmd);

#ifndef	D_BEOBJECT_CTMTASKCMD	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

