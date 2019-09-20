/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CRecordSet		                                             |
|  Task     : CRecordSet header file				                         |
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
#ifndef	_RECORDSET_CONTROL
#define	_RECORDSET_CONTROL

#include	"files.h"
#include	"tmconfig.h"
#include	"sram.h"

/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/

const	int	QUERY_NAMEID_CONDITION												= 1;			//	根據ID
const	int	QUERY_DATE_CONDITION												= 2;			//	某個日期
const	int	QUERY_DATETIME_CONDITION											= 3;			//	某個日期和時間
const	int	QUERY_DATERANGE_CONDITION											= 4;			//	某個日期段
const	int	QUERY_DATETIMERANGE_CONDITION										= 5;			//	某個精確時間段
const	int	QUERY_DBINDEX_CONDITION												= 6;			//	某筆資料index
const	int	QUERY_DBNAME_CONDITION												= 7;			//	某筆資料的名稱

const	int Delete_ALL_CONDITION												= 0;			//	刪除全部
const	int	Delete_NAMEID_CONDITION												= 1;			//	根據ID
const	int	Delete_DATE_CONDITION												= 2;			//	某個日期
const	int	Delete_DATETIME_CONDITION											= 3;			//	某個日期和時間
const	int	Delete_DATERANGE_CONDITION											= 4;			//	某個日期段
const	int	Delete_DATETIMERANGE_CONDITION										= 5;			//	某個精確時間段
const	int	Delete_DBINDEX_CONDITION											= 6;			//	某筆資料index
const	int	Delete_DBNAME_CONDITION												= 7;			//	某筆資料的名稱

/*==========================================================================+
|          Type definition                                                  |
+==========================================================================*/
typedef	struct tagtmModifyRecordSet_Oil
{
	union
	{
		int		nCursor;
		char	szID[10];					//	使用者的ID
	};
	tmDATE	date;
	tmTIME	time;
	int		DBIndex;          			//	修改的資料序號
	long long OldValue;     			//	修改之前的舊值
	long long	NewValue;     			//	修改之後的舊值
}tmModifyRecordSet_Oil, tmModifyRecordSet_Sim;

typedef	struct tagModifyRecordSet_Head
{
	int	nCount;
	int nIndex;
}tmModifyRecordSet_Head;


/*===========================================================================+
|           Class declaration - CRecordSet   	                             |
+===========================================================================*/
class CRecordSet
{
	public:
		virtual	BOOL	SaveModifyRecord(void* pInData)										= 0;
		virtual	BOOL	GetModifyRecordS(int nIndex, void* pOutData)						= 0;
		virtual	int		GetModifyRecord(void* pOutData)										= 0;
		virtual	char*	GetAction(void* pData, int nNum = 0)								= 0;
		virtual	char*	GetAction_FT(void* pData, int nNum = 0)								= 0;
		virtual	int		QueryModifyRecord(int	nCondition, void*	pCondition, void* pOutData) = 0;
		virtual	BOOL	DeleteRecord(int nCondition = 0, void* pCondition = NULL) = 0;
		virtual	int		GetMaxNum()															= 0;
		virtual	int		GetNum()															= 0;
};


/*===========================================================================+
|           Class declaration - COilRecordSet  	                             |
+===========================================================================*/
class COilRecordSet: public CRecordSet
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/	
	public:
		COilRecordSet();
		virtual	~COilRecordSet();
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:       
		BOOL	SaveModifyRecord(void* pInData)																				; 
		BOOL	GetModifyRecordS(int nIndex, void* pOutData)								;
		; 
		int		GetModifyRecord(void* pOutData)												; 
		char*	GetAction(void* pData, int nNum = 0)										;
		char*	GetAction_FT(void* pData, int nNum = 0)										;

		int		QueryModifyRecord(int	nCondition, void*	pCondition, void* pOutData)		;
		BOOL	DeleteRecord(int nCondition = 0, void* pCondition = NULL)					;
		int		GetMaxNum()																	;
		int		GetNum()																	;
	protected:
		int		PerSize()	{return sizeof(tmModifyRecordSet_Oil);};
	private:
		void	OpenFile();
		char*	GetStr(DWORD nIndex);
		
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/	
	protected:
		int		m_nMaxNum;
		int		m_nRecordSetNum;
		bool	m_bSaved;
		CtmRecordFile	m_cFile;
	private:
		tmModifyRecordSet_Oil		m_tmRecordSetCursor;
		bool	m_bFileState;
		char*	m_pszOutAction;
		CtmSarmRecordFileControl* m_pSramRecordFile;
};

/*===========================================================================+
|           Class declaration - CSimRecordSet  	                             |
+===========================================================================*/
//	目前全電和油壓是一致的 
typedef	COilRecordSet CSimRecordSet;


/*===========================================================================+
|           Class declaration - COilRecordSet  	                             |
+===========================================================================*/
class	CRecordSetControl
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/	
	protected:
		virtual	~CRecordSetControl();
		CRecordSetControl();
	public:
		static	CRecordSetControl*	GetInstance();
		static	void		ReleaseInstance();	
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:	
		char	m_pszValue[500];
		BOOL	SaveModifyRecord(void* pInData);
		BOOL	GetModifyRecordS(int nIndex, char** pOutAction, void* pOutData);
		BOOL	GetModifyRecordS_FT(int nIndex, char** pOutAction, void* pOutData);
		int		GetMoidfyRecord(char**	pOutAction, void* pOutData);
		int		QueryModifyRecord(int	nCondition, void*	pCondition, char** pOutAction, void* pOutData);
		BOOL	DeleteRecord(int nCondition = 0, void* pCondition = NULL);
		int		GetMaxNum();
		int		GetNum();
	private:
		static	CRecordSetControl*	m_pInstance;
		static	CRecordSet*	m_pCRecordSet;
};

extern	int			g_RecordFlag;
#endif
