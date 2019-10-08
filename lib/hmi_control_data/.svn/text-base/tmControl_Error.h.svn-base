/*===========================================================================+
|  Class    : CtmControl_Error		                                         |
|  Task     : Alarm Server Function 				    	         		 |
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
#ifndef	D_CTMCONTROL_ERROR
#define	D_CTMCONTROL_ERROR
#include	"../../tmcontrol.h"
#include	"../../tmpackclass.h"
#include	"../../package.h"
#include	"../../tmcontrol.h"

#define 		ERRORLISTMAX_NUM	20

typedef struct tagtmErrorInfo
{
	WORD				wSource;
	WORD				wGrade;
	DWORD				dwErrorID;
	tagtmErrorInfo*		pNext;
} tmErrorInfo;

class	CtmControl_Error: public	CtmControl
{
	DECLARE_DYNCREATE(CtmControl_Error)
	public:
		CtmControl_Error();
		~CtmControl_Error();
		
	public:
		/*virtual*/BOOL    		AddErrorLog(DWORD dwID, WORD	wSource = tmOIL_ERRORFLAG);
		/*virtual*/BOOL    		ClearErrorLog()							;
		/*virtual*/void	   		Destroy_ErrorLog()						;
		/*virtual*/void    		Error_Control()							;
		 		   void    		Error_Control2_Old()					;	
				   void    		Error_Control2_New()					; 
		/*virtual*/BOOL    		FixErrorLog(DWORD dwID, WORD	wSource = tmOIL_ERRORFLAG);
		/*virtual*/void /*tmDB_ERROR*/*	GetErrorIndex()					;
		/*virtual*/void /*tmDB_ERROR*/*	GetErrorLog(int nIndex)			;
		/*virtual*/WORD        	GetErrorLogCount()						;
		/*virtual*/void			Init_ErrorLog()							;
				   void			Error_Control_Card()					;

		/*virtual*/int			  ParseErrorID(char* pszErrorStr, DWORD dwErrorID);//Caozz 2011/4/14 上午 11:34:55
				       int			  ErrorIDToStr(char* szTemp, char* szStrID ,WORD wErrorID, WORD *pwDst);
				   			int				ErrorIDToStr_COM2(char* szTemp, WORD wErrorID, WORD *pwDst);

				       int			  GetTemperErrorStatus()					;
				       int			  GetMoldTemperErrorStatus()				;

		void					Dsp28Error_Control()					;
		void					N2CError_Control()						;
		void	 				RollError()								;
		BOOL 					ChangeErrorList(DWORD dwErrorID, WORD wSource, WORD wGrade, int nFlag);
		
		void    				Error_Control_Old()						;
		void    				Error_Control_New()						;
		void    				Error_Control_COM2_New()			;
	private:
		pthread_mutex_t				m_MutexErrMsg;				//多線程鎖
		static	tmDB_ERROR			m_MsgCurrent;				//當前警報
		static	tmDB_ERROR			m_MsgIndex;					//記錄當前最新的個數和序號
		
		tmErrorInfo*		m_ErrorListHead;			
		tmErrorInfo*		m_ErrorListCurrent;

		static	int			m_nErrorCount;				//
		static	char*		m_pszError[64];
		int					m_iRealIndex;
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
};


#ifndef	D_BEOBJECT_CTMCONTROL_ERROR	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

