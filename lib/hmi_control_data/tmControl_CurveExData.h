/*===========================================================================+
|  Class    : CtmControl_CurveExData			                             |
|  Task     : CurveData Service Function 				    	         	 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)4.3.2                                                  |
|  Link     : G++(GCC)4.3.2                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James	                                                         |
|  Version  : V1.00                                                          |
|  Creation : 07/26/2011	                                                 |
|  Revision : 07/26/2011				                                     |
+===========================================================================*/
#ifndef	D_CTMCONTRL_CURVEEXDATA
#define	D_CTMCONTRL_CURVEEXDATA
#include	"../../tmcontrol.h"
#include	"../../tmpackclass.h"
#include	"../../package.h"
#include	"../../tmcontrol.h"
#include	"../../curvedata.h"
#include	"../libControl.h"

class	CtmControl_CurveExData: public	CtmControl
{
	DECLARE_DYNCREATE(CtmControl_CurveExData)
	public:
		CtmControl_CurveExData();
		~CtmControl_CurveExData();
		
		/*virtual*/void		InitControl()													;
	public:
		/*virtual*/int		SetData(int 	nType, double* dX, double* dY, int nNum)	    ;
		/*virtual*/int		SetData(int	nType, DWORD* dX, DWORD* dY, int nNum)		    	;
		/*virtual*/int		SetData(int	nType, void* pInputData, int nNum)			    	;
		/*virtual*/int		GetData(int 	nType, void* pOutData,int TempDataIndex = 0)	;
		/*virtual*/void		SetComplete(int nType)											;
		/*virtual*/int		SetState(int 	nType, int nState)								;
		/*virtual*/int		GetState(int 	nType)											;
		/*virtual*/void    	ClearBuffer(int nType = -1)                                 	;
		
		/*virtual*/int		GetData2(int 	nType, void* pOutData,int TempDataIndex = 0)	;
		/*virtual*/int		SetData2(int	nType, void* pInputData, int nNum)				;
		/*virtual*/void		SetComplete2(int nType)											;
		/*virtual*/int		SetState2(int 	nType, int nState)								;
		/*virtual*/int		GetState2(int 	nType)											;
		/*virtual*/void    	ClearBuffer2(int nType = -1)                                 	;
		void		SetPointCount(int nType, int nCount)	;
		
		
		int			AddPoint(int nType, void* pData, int nCurveNum);
		int			GetCurveState(int nType);
		BOOL		CurveUpdate(int nType);
		int			GetCurveData(int nType, void *pData, int nPoint, int TempDataIndex = 0);
		int			GetCurveLastIndex(int nType);
		void		Clear(int nType);
		int			CurveNubCovert(int nType);
	protected:
		void		SetCurveState(int nType, int nState);
	private:
		int					m_aryState[MAX_CURVEEX_NUM_NEW];					//存儲曲線狀態
		int					m_aryIndex[MAX_CURVEEX_NUM_NEW];					//存儲曲線序號
		
		int					m_ary2State[MAX_CURVEEX_NUM_NEW];					//存儲曲線狀態
		int					m_ary2Complete[MAX_CURVEEX_NUM_NEW];				//存儲曲線?束
		int					m_ary2Index[MAX_CURVEEX_NUM_NEW];					//存儲曲線序號
		int					m_ary2Count[MAX_CURVEEX_NUM_NEW];					//存儲曲線點數
		
		int					m_ary2LastIndex[CURVE2_LASTCURVE_MAXTYPE];			//存儲曲線序號
		//int					m_ary2LastCount[CURVE2_LASTCURVE_MAXTYPE][MAX_CURVE_TEMPNUM];	//存儲曲線點數
		
		pthread_mutex_t		m_MutexQuality;										//多線程鎖
		tmCurveOilPoint*	m_aryPoints[MAX_CURVEEX_NUM_NEW];					//存儲x,y曲線
		tmCurve2Point*		m_ary2Points[MAX_CURVEEX_NUM_NEW];					//存儲x1,x2,y1,y2曲線
		
		int					m_aryPoints_MaxNum[MAX_CURVEEX_NUM_NEW];
		int					m_ary2Points_MaxNum[MAX_CURVEEX_NUM_NEW];
		//tmCurve2Point		m_ary2LastPoints[CURVE2_LASTCURVE_MAXTYPE][MAX_CURVE_TEMPNUM][MAX_POINT_NUM];
		//tmCurve2FileData	m_ary2LastPoints[CURVE2_LASTCURVE_MAXTYPE][MAX_CURVE_TEMPNUM];
		
		
		tmHYB5CURVE			m_curvedata[MAX_CURVEEX_NUM_NEW];
		tmHYB5CURVE			m_curveLastdata[CURVE2_LASTCURVE_MAXTYPE][MAX_CURVE_TEMPNUM];
		
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
};	

#ifndef	D_BEOBJECT_CTMCONTRL_CURVEEXDATA	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

