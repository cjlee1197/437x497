#ifndef     D_TASKHMI
#define     D_TASKHMI

#include	"../../task.h"
#include	"../../app.h"
#include	"../../tmconfig.h"
#include	"../../tmshmsg.h"

//	MSG_USER_DEFINE=0x10000000




class CtmTaskHmi : public CTask, public CtmPackClass
{
	DECLARE_DYNCREATE(CtmTaskHmi)	
	public:
		CtmTaskHmi();
		~CtmTaskHmi();
	public:
		//====================
		//	override the function that are defined in the CTask
		//====================
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);
		
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		
	private:
		void	InitData(char*	pszRank, char***	pszArys, int wData);
		
		
		void	Init_AxialConfig(int nDealRamp);
		void	Init_CloseMoldConfig(int nDealRamp=0);
		void	Init_OpenMoldConfig(int nDealRamp=0);
		void	Init_InjectConfig(int nDealRamp=0);
		void	Init_HoldConfig(int nDealRamp=0);
		void	Init_ChargeConfig(int nDealRamp=0);
		void	Init_EjtADVConfig(int nDealRamp=0);
		void	Init_EjtRETConfig(int nDealRamp=0);
		void	Init_AirConfig();
		void	Init_CoreConfig();
		
		void	OnFormCreate();
	private:
		int		m_nRampConfig;
		
};

#ifndef	D_BEOBJECT_CTMTASKHMI	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif


