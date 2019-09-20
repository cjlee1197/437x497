/* Copyright (C), 2003-2010, Techmation. Co., Ltd.*/

/*===========================================================================+
|  Class    : Select Edit                                                    |
|  Task     : Select Edit header file                                        |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Fans                                                            |
|  Version  : V1.00                                                          |
|  Creation : 2010-1-6 18:49:24		                                       |
|  Revision :                                                                |
+===========================================================================*/

#ifndef 	D_SELECTEDITB
#define		D_SELECTEDITB

#include	"selectedit.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
const	int	CNT_INT_BIT_SHOW	= 2;
/*===========================================================================+
|           Macro definition                                                 |
+===========================================================================*/

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

/*============================================================================+
|            Function and Class prototype                                     |
+============================================================================*/
/*============================================================================+
|           Class declaration -- CtmListBox                                   |
+============================================================================*/
class		CtmSelectBit  : public CtmSelectEdit
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	DECLARE_DYNCREATE(CtmSelectBit)
	public:
		CtmSelectBit();	
		~CtmSelectBit();
	public:
		//	
		BOOL 	CreateA();
		void 	SetPropertys();
		void 	SetPropertys_Nub();
		
		//	Show Action			
		void    OnGetFocus();
		void	OnLoseFocus();	
		
		
		bool 	SetPropValueT(char* pszPropName, double Value);
		bool 	SetPropValueT(char* pszPropName, char* pszValue);
		
		int		OnLose();		//	fans add 2010-1-15 15:47:07
	
	protected:
		void	GetDBInfo();
		void	SetDBInfo();
	
	protected:
		
		//==========================//
		//	read only attributes
		//==========================//
		long long 	m_llValue;				//	the value of the dbid
		
		
		//==========================//
		//	read & write attributes
		//==========================//
	
		int			m_nBitIndex;			//	the index  of the value's bit
		WORD		m_wBitValue;			//	the bit value:0 or 1
 		char*		m_pszStrs[CNT_INT_BIT_SHOW];
 		char*		m_pszStrIDs[CNT_INT_BIT_SHOW];
 		char*		m_pszDBID;

		
};
#endif
