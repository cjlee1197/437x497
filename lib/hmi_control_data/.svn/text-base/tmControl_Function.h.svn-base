
/*===========================================================================+
|  Class    : CtmControl_FUNCTION			                                 |
|  Task     : Function Service Function 				    	         	 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James	                                                         |
|  Version  : V1.00                                                          |
|  Creation : 01/09/2009	                                                 |
|  Revision : 01/09/2009				                                     |
+===========================================================================*/
#ifndef	D_CTMCONTROL_FUNCTION
#define	D_CTMCONTROL_FUNCTION
#include	"../../tmcontrol.h"
#include	"../../tmpackclass.h"
#include	"../../package.h"
#include	"../../tmcontrol.h"

class	CtmControl_Function: public	CtmControl
{
	DECLARE_DYNCREATE(CtmControl_Function)
	public:
		CtmControl_Function();
		~CtmControl_Function();
		
	public:
		/*virtual*/void	InitControl();
	public:
		/*virtual*/void		_OnTimeHeater();
		/*virtual*/void		_CountZero();
		/*virtual*/void		_ClearZeroOil();
		
		/*virtual*/void    	_GetInitPasswd_Install(char* pcMachineID, char* pcPassword, 
											tmDATE dtMachineDateOut);
		/*virtual*/int      _GetStage_Install(char* pcPassword, char* pcMachineID, 
											tmDATE dtStartStage);
		/*virtual*/BOOL    	_CheckTimeOut_Install();
		/*virtual*/WORD   	_StartUp_Install(char* pcPassword);
		/*virtual*/WORD		_JudgeFactoryOnWork_Install();
		/*virtual*/void		_SaveOperation();
		/*virtual*/void		_DealOperation(OPERATION* pOperation, int nWRflag);
		/*virtual*/int		_ReadOperationFromSRAM(OPERATION* pOperation);
		/*virtual*/int		_WriteOperationToSRAM(OPERATION* pOperation);
		/*virtual*/void		_DealOperationStartMachine();
		/*virtual*/WORD		_CountOperationCRC();
		/*virtual*/int		_GetBlockValueState(int nBlockID, tmStorageType StorageType);
		/*virtual*/WORD		_VER2HEX();
		
		/*virtual*/void     _SetCoreEffectPN(BOOL bSendDSP);
		/*virtual*/void     _SetEjectEffectPN(BOOL bSendDSP);

		/*virtual*/void     _SetCoreEffectPN2(BOOL bSendDSP);
		/*virtual*/void     _SetEjectEffectPN2(BOOL bSendDSP);

		void				SaveCurrentRTCTime(OPERATION* pOperation);
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
};	

#ifndef	D_BEOBJECT_CTMCONTROL_FUNCTION	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

