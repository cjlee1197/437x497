/*===========================================================================+
|  Class    : CtmControl_CurveData			                                 |
|  Task     : CurveData Service Function 				    	         	 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James	                                                         |
|  Version  : V1.00                                                          |
|  Creation : 12/22/2008	                                                 |
|  Revision : 12/22/2008				                                     |
+===========================================================================*/
#ifndef	D_CTMCONTRL_CURVEDATA
#define	D_CTMCONTRL_CURVEDATA
#include	"../../tmcontrol.h"
#include	"../../tmpackclass.h"
#include	"../../package.h"
#include	"../../tmcontrol.h"
#include	"../../curvedata.h"
#include	"../libControl.h"

class	CtmControl_CurveData: public	CtmControl
{
	DECLARE_DYNCREATE(CtmControl_CurveData)
	public:
		CtmControl_CurveData();
		~CtmControl_CurveData();
		
	public:
		/*virtual*/BOOL		AddControl(void *pParas[])						;
		/*virtual*/BOOL		ModifyControl(void *pParas[]) 					;
		/*virtual*/BOOL		FindControl(void *pParas[], void *pFindPara)	;
		/*virtual*/BOOL		DeleteControl(void *pDeletePara = NULL)			;
		/*virtual*/BOOL		FixControl(void *pFixPara)						;
		/*virtual*/void		InitControl()									;
		/*virtual*/int		SetData(int nType, double* dX, double* dY, int nNum)	    ;
		/*virtual*/int		SetData(int	nType, DWORD* dX, DWORD* dY, int nNum)		    ;
		/*virtual*/int		SetData(int	nType, void* pInputData, int nNum)			    ;
		/*virtual*/int		GetData(int nType, void* pOutData,int TempDataIndex = 0)	;
		/*virtual*/void		SetComplete(int nType)										;
		/*virtual*/int		SetState(int nType, int nState)								;
		/*virtual*/int		GetState(int nType)											;
		/*virtual*/void    	ClearBuffer(int nType = -1)                                 ;
	protected:
		//Inject A
		int		GetInjectPress(void* pOutData)	;
		int 	GetInjectSpeed(void* pOutData)	;
		int		GetInjectPos(void* pOutData)	;
		int		GetHoldPress(void* pOutData)	;
		int 	GetHoldSpeed(void* pOutData)	;
		int		GetChargePress(void* pOutData)	;  //Sunny<20070815>
		int		GetChargeBackPres(void* pOutData); //Sunny<20070815>
		int		GetChargeRPM(void* pOutData)	;
		int		GetMaxPosData();
		//Inject B
		int		GetInjectBPress(void* pOutData)	;
		int 	GetInjectBSpeed(void* pOutData)	;
		int		GetInjectBPos(void* pOutData)	;
		int		GetHoldBPress(void* pOutData)	;
		int 	GetHoldBSpeed(void* pOutData)	;
		int		GetChargeBPress(void* pOutData)	;
		int		GetChargeBBackPres(void* pOutData);
		int		GetChargeBRPM(void* pOutData)	;
		int		GetMaxPosBData();
		int		CurveNubCovert(int nType);
	private:
		int		m_aryState[MAX_CURVE_NUM_NEW];
		int		m_aryIndex[MAX_CURVE_NUM_NEW];
		int		m_aryComplete[MAX_CURVE_NUM_NEW];
		pthread_mutex_t		m_MutexQuality;				//¦h½uµ{Âê
		tmCurveOilPoint*		m_aryPoints[MAX_CURVE_NUM_NEW];
		int		m_aryPoints_MaxNum[MAX_CURVE_NUM_NEW];
		//tmOilCurveFileData	m_oilData;
		CtmSarmRecordFileControl*	m_pCurveFile;		//	save moldset's infomation, include head and content	m_oilData;
		static	tmOilCurveFileData	m_aryLastCurvePoint[MAX_CURVE_NUM_NEW][MAX_CURVE_TEMPNUM];
		static  int m_LastCurvePointIndex[MAX_CURVE_NUM_NEW];

	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
};	

#ifndef	D_BEOBJECT_CTMCONTRL_CURVEDATA	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

