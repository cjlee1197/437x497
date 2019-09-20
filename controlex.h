/*===========================================================================+
|  Function : Control                                                        |
|  Task     : Machine Control Utilities header file                          |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.52                                                     |
|  Link     : MSVC V1.52                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : shousu fan                                                     |
|  Version  : V1.00                                                          |
|  Creation :						                                                     |
|  Revision :						                                                     |
+===========================================================================*/


#ifndef     D_CONTROL_EX
#define     D_CONTROL_EX

#include	"common.h"

//	control error, include 3 classes, real controler is CtmErrorControl;
class	CtmError
{
	struct	tmERROR
	{
	};
	public:
		void				Init_ErrorLog()								= 0;
		void				Destroy_ErrorLog()						= 0;
		void        Error_Control()								= 0;
		WORD        GetErrorLogCount()						= 0;
		void*				GetErrorLog(int nIndex)				= 0;	  
		void*				GetErrorIndex()								= 0;
		BOOL        AddErrorLog(WORD wID)					= 0;
		BOOL        FixErrorLog(WORD wID)					= 0;
		BOOL        ClearErrorLog()								= 0;
		
};

class CtmOilError: CtmError
{
	struct tmERROR: CtmError::tmERROR
  {
		WORD			wID;                                	// 故障代碼
		DWORD			dwShotCount;                        	// 開模序號
		tmDATE		dateStart;                          // 故障起始日期
		tmTIME		timeStart;                          // 故障起始時間
		tmDATE		dateFixed;                          // 故障排除日期
		tmTIME		timeFixed;                          // 故障排除時間
  };
  
	public:
		void				Init_ErrorLog()									;
		void				Destroy_ErrorLog()							;
		void        Error_Control()									;
		WORD        GetErrorLogCount()							;
		void*				GetErrorLog(int nIndex)					;	  
		void*				GetErrorIndex()									;
		BOOL        AddErrorLog(WORD wID)						;
		BOOL        FixErrorLog(WORD wID)						;
		BOOL        ClearErrorLog()									;	
	
};

class CtmSimError:CtmError
{
	struct tmERROR: CtmERROR::tmERROR
	{
		BYTE		bMsgType;
		BYTE		bState;
		DWORD		dwTime;
		DWORD		dwDate;
		WORD		wObjNumber;
		union
		{
			char	cData[16];
			struct
			{
				long	lFO_Error;
				long	lFO_ErrorID1;
				long	lFO_ErrorID2;
				long	lFO_ErrorID3;
			};
			long	lAlarmNo;
			DWORD	dwInitEvent;
			DWORD	dwExecFaultType;
			struct
			{
				DWORD	dwPH_InitEvent;
				long	lPH_LogBaseAdrIn;
				long	lPH_LogBaseAdrOut;
				long	lPH_LogDiagAdr;
			};
			struct
			{
				long	lErrorCode;
				short	nSizeRet;
				short	nTypeRet;
				short	nCounterRet;
				short	nCycleRet;
				short	nNoElementsRet;
			};
			WORD		wErrorID;
			struct
			{
				long	lDrvErrorID;
				long	lDrvError;
			};
			struct
			{
				long	lAppError;
				long	lAppErrorID1;
				long	lAppErrorID2;
			};
		};
	};
	
	public:
		void				Init_ErrorLog()									;
		void				Destroy_ErrorLog()							;
		void        Error_Control()									;
		WORD        GetErrorLogCount()							;
		void*				GetErrorLog(int nIndex)					;	  
		void*				GetErrorIndex()									;
		BOOL        AddErrorLog(WORD wID)						;
		BOOL        FixErrorLog(WORD wID)						;
		BOOL        ClearErrorLog()									;	
};



class	CtmErrorControl
{
	typedef	tmERROR	m_cError::tmERROR;
	
	public:
		void				Init_ErrorLog();
		void				Destroy_ErrorLog();
		void        Error_Control();
		WORD        GetErrorLogCount();
		void*				GetErrorLog(int nIndex);   // wIndex = 1 is the first (nearest) record
		void*				GetErrorIndex();
		BOOL        AddErrorLog(WORD wID);
		BOOL        FixErrorLog(WORD wID);
		BOOL        ClearErrorLog();
		
	protected:
		static	CtmError*		m_cError;
};



//	control moni,	real controller is CtmMoniControl;
class	CtmMoni
{
	public:
		void		Init_MoniLog()								= 0;
		void		Destroy_MoniLog()							= 0;
		int			GetMoniLogCount()							= 0;
		void*		GetMoniLog(int nIndex)				= 0;
		void*		GetMoniIndex()								= 0;
		BOOL		AddMoni2()										= 0;
		BOOL    ClearMoniLog()								= 0;
		BOOL		SaveMoni2(void* pMoni2)				= 0;
};

class CtmOilMoni:CtmMoni
{
	typedef struct tagtmOIL_MONI2
  {
      DWORD   dwShotCount;
      _TIMEL  tmlCycle;                           // 600 循環計時實際值    
      _TIMEL  tmlInject;                          // 606 射出計時實際值 
      _TIMEL  tmllTurnToHold;                     // 608 保壓轉換時間實際值
      _TIMEL  tmlCharge;                          // 60D 儲料計時實際值 
      _DA     daPres_TurnToHold;                  // 612 保壓轉換壓力實際值
      _AD     adPosi_InjectStart;                 // 609 射出起點
      _AD     adPosi_TurnToHold;                  // 60A 保壓起點
      _AD     adPosi_InjectEnd;                   // 60C 射出終點
      _AD     adPosi_InjectCushion;               // 60C 射出監控位置
      _COUNT  cnChargeRPM;                        // 60D 儲料RPM 實際值
      _TIMER  tmSuckBack;                         // 60D 射退計時實際值
      _TIMER  tmEject;                            // 60D 脫模計時實際值
      _TIMER  tmEjectAdvance;                     // 60D 脫進計時實際值
      _TIMER  tmEjectReturn;                      // 60D 脫退計時實際值
      _DA     daPres_InjectPeak;                  // 612 射出尖峰壓力
      _DA     daPres_ChargePeak;                  // 612 儲料尖峰壓力 
      _TIMEL  tmlInjectB;                          // 660 B組射出計時實際值
      _TIMEL  tmlChargeB;                          // 667 B組儲料計時實際值
      _AD     adPosi_InjectStartB;                // 663 B組射出起點
      _AD     adPosi_TurnToHoldB;                 // 664 B組保壓起點
      _AD     adPosi_InjectCushionB;              // 666 B組射出終點
      _DA     daPres_InjecterB;              // 66C 射出壓力實際值   //gwei 2003/7/23
      _DA     daPres_ChargeBackB;            // 66D 儲料背壓實際值   //gwei 2003/7/23
      WORD    wReservedMG[6];                     // 00F 保留00F-017
      
      //	fans	2006/12/13 08:29下午
      tagtmOIL_MONI2 operator+(tagtmOIL_MONI2 moni1)
      {
      	tagtmOIL_MONI2 Result;
      	Result.tmlCycle 								= moni1.tmlCycle 							+ this->tmlCycle;
      	Result.tmlInject 								= moni1.tmlInject 						+ this->tmlInject;
      	Result.tmllTurnToHold						= moni1.tmllTurnToHold 				+ this->tmllTurnToHold;
      	Result.tmlCharge								= moni1.tmlCharge 						+ this->tmlCharge;
      	Result.daPres_TurnToHold				= moni1.daPres_TurnToHold 		+ this->daPres_TurnToHold;
      	Result.adPosi_InjectStart				= moni1.adPosi_InjectStart 		+ this->adPosi_InjectStart;
      	Result.adPosi_TurnToHold				= moni1.adPosi_TurnToHold 		+ this->adPosi_TurnToHold;
      	Result.adPosi_InjectEnd					= moni1.adPosi_InjectEnd 			+ this->adPosi_InjectEnd;
      	Result.adPosi_InjectCushion			= moni1.adPosi_InjectCushion	+ this->adPosi_InjectCushion;
      	Result.cnChargeRPM							= moni1.cnChargeRPM 					+ this->cnChargeRPM;
      	Result.tmSuckBack								= moni1.tmSuckBack 						+ this->tmSuckBack;
      	Result.tmEject									= moni1.tmEject 							+ this->tmEject;
      	Result.tmEjectAdvance						= moni1.tmEjectAdvance 				+ this->tmEjectAdvance;
      	Result.tmEjectReturn						= moni1.tmEjectReturn 				+ this->tmEjectReturn;
      	Result.daPres_InjectPeak				= moni1.daPres_InjectPeak 		+ this->daPres_InjectPeak;
      	Result.daPres_ChargePeak				= moni1.daPres_ChargePeak 		+ this->daPres_ChargePeak;
      	Result.tmlInjectB								= moni1.tmlInjectB 						+ this->tmlInjectB;
      	Result.tmlChargeB								= moni1.tmlChargeB 						+ this->tmlChargeB;
      	Result.adPosi_InjectStartB			= moni1.adPosi_InjectStartB 	+ this->adPosi_InjectStartB;
      	Result.adPosi_TurnToHoldB				= moni1.adPosi_TurnToHoldB 		+ this->adPosi_TurnToHoldB;
      	Result.adPosi_InjectCushionB		= moni1.adPosi_InjectCushionB + this->adPosi_InjectCushionB;
      	Result.daPres_InjecterB					= moni1.daPres_InjecterB 			+ this->daPres_InjecterB;
      	Result.daPres_ChargeBackB				= moni1.daPres_ChargeBackB 		+ this->daPres_ChargeBackB;
      	return Result;
      }
       tagtmOIL_MONI2 operator-(tagtmOIL_MONI2 moni1)
      {
      	tagtmOIL_MONI2 Result;
      	Result.tmlCycle 								= this->tmlCycle              - moni1.tmlCycle 							;
      	Result.tmlInject 								= this->tmlInject             - moni1.tmlInject 						;
      	Result.tmllTurnToHold						= this->tmllTurnToHold        - moni1.tmllTurnToHold 				;
      	Result.tmlCharge								= this->tmlCharge             - moni1.tmlCharge 						;
      	Result.daPres_TurnToHold				= this->daPres_TurnToHold     - moni1.daPres_TurnToHold 		;
      	Result.adPosi_InjectStart				= this->adPosi_InjectStart    - moni1.adPosi_InjectStart 		;
      	Result.adPosi_TurnToHold				= this->adPosi_TurnToHold     - moni1.adPosi_TurnToHold 		;
      	Result.adPosi_InjectEnd					= this->adPosi_InjectEnd      - moni1.adPosi_InjectEnd 			;
      	Result.adPosi_InjectCushion			= this->adPosi_InjectCushion  - moni1.adPosi_InjectCushion	;
      	Result.cnChargeRPM							= this->cnChargeRPM           - moni1.cnChargeRPM 					;
      	Result.tmSuckBack								= this->tmSuckBack            - moni1.tmSuckBack 						;
      	Result.tmEject									= this->tmEject               - moni1.tmEject 							;
      	Result.tmEjectAdvance						= this->tmEjectAdvance        - moni1.tmEjectAdvance 				;
      	Result.tmEjectReturn						= this->tmEjectReturn         - moni1.tmEjectReturn 				;
      	Result.daPres_InjectPeak				= this->daPres_InjectPeak     - moni1.daPres_InjectPeak 		;
      	Result.daPres_ChargePeak				= this->daPres_ChargePeak     - moni1.daPres_ChargePeak 		;
      	Result.tmlInjectB								= this->tmlInjectB            - moni1.tmlInjectB 						;
      	Result.tmlChargeB								= this->tmlChargeB            - moni1.tmlChargeB 						;
      	Result.adPosi_InjectStartB			= this->adPosi_InjectStartB   - moni1.adPosi_InjectStartB 	;
      	Result.adPosi_TurnToHoldB				= this->adPosi_TurnToHoldB    - moni1.adPosi_TurnToHoldB 		;
      	Result.adPosi_InjectCushionB		= this->adPosi_InjectCushionB - moni1.adPosi_InjectCushionB ;
      	Result.daPres_InjecterB					= this->daPres_InjecterB      - moni1.daPres_InjecterB 			;
      	Result.daPres_ChargeBackB				= this->daPres_ChargeBackB    - moni1.daPres_ChargeBackB 		;
      	return Result;
      }
      tagtmOIL_MONI2 operator*(tagtmOIL_MONI2 moni1)
      {
      	tagtmOIL_MONI2 Result;
      	Result.tmlCycle 								= moni1.tmlCycle 							* this->tmlCycle;
      	Result.tmlInject 								= moni1.tmlInject 						* this->tmlInject;
      	Result.tmllTurnToHold						= moni1.tmllTurnToHold 				* this->tmllTurnToHold;
      	Result.tmlCharge								= moni1.tmlCharge 						* this->tmlCharge;
      	Result.daPres_TurnToHold				= moni1.daPres_TurnToHold 		* this->daPres_TurnToHold;
      	Result.adPosi_InjectStart				= moni1.adPosi_InjectStart 		* this->adPosi_InjectStart;
      	Result.adPosi_TurnToHold				= moni1.adPosi_TurnToHold 		* this->adPosi_TurnToHold;
      	Result.adPosi_InjectEnd					= moni1.adPosi_InjectEnd 			* this->adPosi_InjectEnd;
      	Result.adPosi_InjectCushion			= moni1.adPosi_InjectCushion	* this->adPosi_InjectCushion;
      	Result.cnChargeRPM							= moni1.cnChargeRPM 					* this->cnChargeRPM;
      	Result.tmSuckBack								= moni1.tmSuckBack 						* this->tmSuckBack;
      	Result.tmEject									= moni1.tmEject 							* this->tmEject;
      	Result.tmEjectAdvance						= moni1.tmEjectAdvance 				* this->tmEjectAdvance;
      	Result.tmEjectReturn						= moni1.tmEjectReturn 				* this->tmEjectReturn;
      	Result.daPres_InjectPeak				= moni1.daPres_InjectPeak 		* this->daPres_InjectPeak;
      	Result.daPres_ChargePeak				= moni1.daPres_ChargePeak 		* this->daPres_ChargePeak;
      	Result.tmlInjectB								= moni1.tmlInjectB 						* this->tmlInjectB;
      	Result.tmlChargeB								= moni1.tmlChargeB 						* this->tmlChargeB;
      	Result.adPosi_InjectStartB			= moni1.adPosi_InjectStartB 	* this->adPosi_InjectStartB;
      	Result.adPosi_TurnToHoldB				= moni1.adPosi_TurnToHoldB 		* this->adPosi_TurnToHoldB;
      	Result.adPosi_InjectCushionB		= moni1.adPosi_InjectCushionB * this->adPosi_InjectCushionB;
      	Result.daPres_InjecterB					= moni1.daPres_InjecterB 			* this->daPres_InjecterB;
      	Result.daPres_ChargeBackB				= moni1.daPres_ChargeBackB 		* this->daPres_ChargeBackB;
      	return Result;
      }
      tagtmOIL_MONI2 operator/(double d1)
      {
      	tagtmOIL_MONI2 Result;
      	Result.tmlCycle 								= ( _TIMEL)(this->tmlCycle              / d1);
      	Result.tmlInject 								= ( _TIMEL)(this->tmlInject             / d1);
      	Result.tmllTurnToHold						= ( _TIMEL)(this->tmllTurnToHold        / d1);
      	Result.tmlCharge								= ( _TIMEL)(this->tmlCharge             / d1);
      	Result.daPres_TurnToHold				= ( _DA   )(this->daPres_TurnToHold     / d1);
      	Result.adPosi_InjectStart				= ( _AD   )(this->adPosi_InjectStart    / d1);
      	Result.adPosi_TurnToHold				= ( _AD   )(this->adPosi_TurnToHold     / d1);
      	Result.adPosi_InjectEnd					= ( _AD   )(this->adPosi_InjectEnd      / d1);
      	Result.adPosi_InjectCushion			= ( _AD   )(this->adPosi_InjectCushion  / d1);
      	Result.cnChargeRPM							= ( _COUNT)(this->cnChargeRPM           / d1);
      	Result.tmSuckBack								= ( _TIMER)(this->tmSuckBack            / d1);
      	Result.tmEject									= ( _TIMER)(this->tmEject               / d1);
      	Result.tmEjectAdvance						= ( _TIMER)(this->tmEjectAdvance        / d1);
      	Result.tmEjectReturn						= ( _TIMER)(this->tmEjectReturn         / d1);
      	Result.daPres_InjectPeak				= ( _DA   )(this->daPres_InjectPeak     / d1);
      	Result.daPres_ChargePeak				= ( _DA   )(this->daPres_ChargePeak     / d1);
      	Result.tmlInjectB								= ( _TIMEL)(this->tmlInjectB            / d1);
      	Result.tmlChargeB								= ( _TIMEL)(this->tmlChargeB            / d1);
      	Result.adPosi_InjectStartB			= ( _AD   )(this->adPosi_InjectStartB   / d1);
      	Result.adPosi_TurnToHoldB				= ( _AD   )(this->adPosi_TurnToHoldB    / d1);
      	Result.adPosi_InjectCushionB		= ( _AD   )(this->adPosi_InjectCushionB / d1);
      	Result.daPres_InjecterB					= ( _DA   )(this->daPres_InjecterB      / d1);
      	Result.daPres_ChargeBackB				= ( _DA   )(this->daPres_ChargeBackB    / d1);
      	return Result;
      }
      tagtmOIL_MONI2 operator*(int n1)
      {
      	tagtmOIL_MONI2 Result;
      	Result.tmlCycle 								= ( _TIMEL)(this->tmlCycle              * n1);
      	Result.tmlInject 								= ( _TIMEL)(this->tmlInject             * n1);
      	Result.tmllTurnToHold						= ( _TIMEL)(this->tmllTurnToHold        * n1);
      	Result.tmlCharge								= ( _TIMEL)(this->tmlCharge             * n1);
      	Result.daPres_TurnToHold				= ( _DA   )(this->daPres_TurnToHold     * n1);
      	Result.adPosi_InjectStart				= ( _AD   )(this->adPosi_InjectStart    * n1);
      	Result.adPosi_TurnToHold				= ( _AD   )(this->adPosi_TurnToHold     * n1);
      	Result.adPosi_InjectEnd					= ( _AD   )(this->adPosi_InjectEnd      * n1);
      	Result.adPosi_InjectCushion			= ( _AD   )(this->adPosi_InjectCushion  * n1);
      	Result.cnChargeRPM							= ( _COUNT)(this->cnChargeRPM           * n1);
      	Result.tmSuckBack								= ( _TIMER)(this->tmSuckBack            * n1);
      	Result.tmEject									= ( _TIMER)(this->tmEject               * n1);
      	Result.tmEjectAdvance						= ( _TIMER)(this->tmEjectAdvance        * n1);
      	Result.tmEjectReturn						= ( _TIMER)(this->tmEjectReturn         * n1);
      	Result.daPres_InjectPeak				= ( _DA   )(this->daPres_InjectPeak     * n1);
      	Result.daPres_ChargePeak				= ( _DA   )(this->daPres_ChargePeak     * n1);
      	Result.tmlInjectB								= ( _TIMEL)(this->tmlInjectB            * n1);
      	Result.tmlChargeB								= ( _TIMEL)(this->tmlChargeB            * n1);
      	Result.adPosi_InjectStartB			= ( _AD   )(this->adPosi_InjectStartB   * n1);
      	Result.adPosi_TurnToHoldB				= ( _AD   )(this->adPosi_TurnToHoldB    * n1);
      	Result.adPosi_InjectCushionB		= ( _AD   )(this->adPosi_InjectCushionB * n1);
      	Result.daPres_InjecterB					= ( _DA   )(this->daPres_InjecterB      * n1);
      	Result.daPres_ChargeBackB				= ( _DA   )(this->daPres_ChargeBackB    * n1);
      	return Result;
      }
      void	OilSqrt()
      {        
      	sqrt((double)this->tmlCycle             );
      	sqrt((double)this->tmlInject            );
      	sqrt((double)this->tmllTurnToHold       );
      	sqrt((double)this->tmlCharge            );
      	sqrt((double)this->daPres_TurnToHold    );
      	sqrt((double)this->adPosi_InjectStart   );
      	sqrt((double)this->adPosi_TurnToHold    );
      	sqrt((double)this->adPosi_InjectEnd     );
      	sqrt((double)this->adPosi_InjectCushion );
      	sqrt((double)this->cnChargeRPM          );
      	sqrt((double)this->tmSuckBack           );
      	sqrt((double)this->tmEject              );
      	sqrt((double)this->tmEjectAdvance       );
      	sqrt((double)this->tmEjectReturn        );
      	sqrt((double)this->daPres_InjectPeak    );
      	sqrt((double)this->daPres_ChargePeak    );
      	sqrt((double)this->tmlInjectB           );
      	sqrt((double)this->tmlChargeB           );
      	sqrt((double)this->adPosi_InjectStartB  );
      	sqrt((double)this->adPosi_TurnToHoldB   );
      	sqrt((double)this->adPosi_InjectCushionB);
      	sqrt((double)this->daPres_InjecterB     );
      	sqrt((double)this->daPres_ChargeBackB   );
      }
  } tmMONI;
 
 	public:
 		void		Init_MoniLog()									;
		void		Destroy_MoniLog()								;
		int			GetMoniLogCount()								;
		void*		GetMoniLog(int nIndex)					;
		void*		GetMoniIndex()									;
		BOOL		AddMoni2()											;
		BOOL    ClearMoniLog()									;
		BOOL		SaveMoni2(void* pMoni2)					;
};

class CtmSimMoni:CtmMoni
{
	typedef struct tagI32_MONI
	{  
    DWORD      tmCycleTime;                     //  0 循環計時實際值
    DWORD      tmClampClose;                   	//  1 關模計時實際值
    DWORD      tmClampOpen;                    	//  2 開模計時實際值
    DWORD      tmInject;                       	//  4 射出計時實際值
    DWORD      tmCharge;                       	//  5 儲料計時實際值
    DWORD      tmSuckBack;                     	//  6 射退計時實際值
    DWORD      tmEject;                        	//  7 脫模計時實際值
    double     adPosi_ClampOpenEnd;          	//  8 開模位置實際值
    double     daPres_TurnToHold;               //  9 保壓轉換壓力實際值
    double     tmTurnToHold;                    // 10 保壓轉換時間實際值
    double     adPosi_TurnToHold;               // 11 保壓轉換位置實際值
    double     adPosi_InjectStart;          	// 12 射出起點位置實際值
    double     adPosi_InjectHoldEnd;            // 13 射出終點位置實際值
	} tmMONI;
	
	public:
 		void		Init_MoniLog()									;
		void		Destroy_MoniLog()								;
		int			GetMoniLogCount()								;
		void*		GetMoniLog(int nIndex)					;
		void*		GetMoniIndex()									;
		BOOL		AddMoni2()											;
		BOOL    ClearMoniLog()									;
		BOOL		SaveMoni2(void* pMoni2)					;
};




