/*===========================================================================+
|  Function : tminit                                                      	 |
|  Task     : tminit Utilities header file		                             |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 06/05/2008                                                     |
|  Revision : 06/05/2008                                                     |
+===========================================================================*/

#ifndef     D_TMINIT
#define     D_TMINIT

#include	"../../init.h"
#include	"../../utils.h"
#include	"../../tmconfig.h"
#include	"../../gui_ex.h"
#include	"../../moldControl.h"


/*===========================================================================+
|           Class declaration - CtmInit		                     			 |
+===========================================================================*/
class 	CtmInit: public CInit
{
	DECLARE_DYNCREATE(CtmInit)
	public:
		CtmInit();
		~CtmInit();
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		
		int		Init()										;
		void	Init_System()								;
		int		Init_Bmp()									;
		int		Init_DB()									;
		int		Init_Block()								;
		void	Init_AxialConfig(int nDealRamp)				;
		void	Init_Mold()									;
		void	Init_FlashEX()								;
		void	Init_SramFromBack()							;
		int		Destroy()									;
		int 	Destroy_System()							;
		int 	Destroy_DB()								;
		int 	Destroy_Bmp()								;
	
		int 	Destroy_Block()								;

	public:
		static	CtmInit*	m_pSelf;
		void	Init_CloseMoldConfig(int nDealRamp=0);
		void	Init_OpenMoldConfig(int nDealRamp=0);
		void	Init_InjectConfig(int nDealRamp=0);
		void	Init_HoldConfig(int nDealRamp=0);
		void	Init_ChargeConfig(int nDealRamp=0);
		void	Init_InjectBConfig(int nDealRamp=0);
		void	Init_HoldBConfig(int nDealRamp=0);
		void	Init_ChargeBConfig(int nDealRamp=0);
		void	Init_EjtADVConfig(int nDealRamp=0);
		void	Init_EjtRETConfig(int nDealRamp=0);
		void	Init_AirConfig();
		void	Init_CoreConfig();
		void	Init_CloseInject();
		void	InitData(char*	pszRank, char***	pszArys, int wData);
		void	InitDataCoreSlow(char*	pszRank, char***	pszArys, int wData);
		void  OpenBattery_2();
		void	SetInstallType();
	protected:
		int	Init_Resource();
		int	Destroy_Resource();
};

extern	void		LCDPowerOn_3354(int nCount);


#ifndef	D_BEOBJECT_CtmInit	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
