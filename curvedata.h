
/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmCurveData		                                             |
|  Task     : CtmCurveData header file				                         |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision : 							                                     |
+===========================================================================*/
#ifndef	D_CURVEDATA
#define	D_CURVEDATA
#include	"curve.h"
#include	"sram.h"
#include	"main.h"
#include	"tmcontrol.h"

/*===========================================================================+
|           Class declaration - CtmCurveData                                 |
+===========================================================================*/
class	CtmCurveData
{
	public:
		virtual	int		SetData(int nType, double* dX, double* dY, int nNum)	 	= 0;		//	get index
		virtual	int		SetData(int	nType, DWORD* dX, DWORD* dY, int nNum)		 	= 0;		//	get index
		virtual	int		SetData(int	nType, void* pInputData, int nNum)			 	= 0;
		virtual	int		GetData(int nType, void* pOutDataint,int TempDataIndex = 0) = 0;
		virtual	void	SetComplete(int nType)									 	= 0;
		virtual	int		SetState(int nType, int nState)							 	= 0;
		virtual	int		GetState(int nType)										 	= 0;
		virtual	void	ClearBuffer(int nType = -1)									= 0;
};



/*===========================================================================+
|           Class declaration - CtmCurveOilData                              |
+===========================================================================*/
class	CtmCurveOilData:public	CtmCurveData 
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/	
	public:
		CtmCurveOilData();
		virtual	~CtmCurveOilData();
		
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		virtual int		SetData(int nType, double* dX, double* dY, int nNum)	    ;
		virtual int		SetData(int	nType, DWORD* dX, DWORD* dY, int nNum)		    ;
		virtual int		SetData(int	nType, void* pInputData, int nNum)			    ;
		virtual int		GetData(int nType, void* pOutData,int TempDataIndex = 0)	;
		virtual void	SetComplete(int nType)										;
		virtual int		SetState(int nType, int nState)								;
		virtual int		GetState(int nType)											;
		virtual void    ClearBuffer(int nType = -1)                                 ;
};


/*===========================================================================+
|           Class declaration - CtmCurveDataControl                          |
+===========================================================================*/
class	CtmCurveDataControl
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/	
	public:
		static	CtmCurveDataControl*	GetInstance();
		static	void					ReleaseInstance();
	protected:
		CtmCurveDataControl();
		virtual	~CtmCurveDataControl();
		
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		int		SetData(int nType, double* dX, double* dY, int nNum)	;
		int		SetData(int	nType, DWORD* dX, DWORD* dY, int nNum)		;
		int		SetData(int	nType, void* pInputData, int nNum)		    ;
		int		GetData(int nType, void* pOutData,int TempDataIndex = 0);	
		void	SetComplete(int nType)									;
		int		SetState(int nType, int nState)							;
		int		GetState(int nType)										;
		void    ClearBuffer(int nType=-1)              					;
		
	protected:
	/*------------------------------------------------------------------------------+
	|           Attributes 			                                                 |
	+------------------------------------------------------------------------------*/	
	private:
		static	CtmCurveDataControl*	m_pInstance;
		static	CtmCurveData*			m_pCurveData;
};

#endif
