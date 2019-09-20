#ifndef     D__TMVALUES
#define     D__TMVALUES
#include	"wnd.h"

const	int	CNT_INT_MAX_VALUES_NUM = 256;

class	  CtmValueBase:public CtmWnd
{
	DECLARE_DYNAMIC(CtmValueBase)
	public:

		CtmValueBase();
		~CtmValueBase();
	public:
		tmPropInfo 		ValPropertys[CNT_INT_MAX_VALUES_NUM];
	public:
		tmPropInfo*	GetProperty(int nNub);
		tmPropInfo* 	GetProperty(char* pszPropName);
		void*			GetPropValue(char* pszPropName);		
		void* 		GetPropValue(int nNub);	
		bool			SetProperty(char* pszPropName, char* pszValue);
		bool			SetProperty(int nNub, char* pszValue);
		bool 			SetProperty(char* pszPropName, void* pValue);
		bool 			SetProperty(int nNub, void* pValue);
		bool 			SetPropValueT(char* pszPropName, double Value);
		bool 			SetPropValueT(int nNub, double Value);
		bool 			SetPropValueT(char* pszPropName, char* pszValue, int nLen, int nType);		
		bool 			SetPropValueT(char* pszPropName, char* pszValue);
		bool 			SetPropValueT(int nNub, char* pszValue);
		bool 			GetPropValueT(char* pszPropName, void* pValue, int nDataSize = -1);
		bool 			GetPropValueT(int nNub, void* pValue, int nDataSize = -1);
	protected:
		void 		InitProps();
		int		FindProperty(char* pszPropertyName);
		int		FindProperty(int nNub);
		void 		SetPropInfo(char* pszName, PropType PType, void* PropAddress);
		void 		SetPropInfo(int nNub, PropType PType, void* PropAddress);
		int		GetPropCount();
};

class	CtmValueStr: public CtmValueBase
{
	DECLARE_DYNCREATE(CtmValueStr)
	public:
		CtmValueStr();
		~CtmValueStr();
	public:
		BOOL 	CreateA();
		void	SetPropertys();
		void	SetPropertys_Nub();
		int		GetValuesNum();
	protected:
		char*	m_pValues[CNT_INT_MAX_VALUES_NUM];		
		char*	m_pszDefault;
};


class	CtmValueInt: public CtmValueBase
{
	DECLARE_DYNCREATE(CtmValueInt)
	public:
		CtmValueInt();
		~CtmValueInt();
	public:
		BOOL 	CreateA();
		void	SetPropertys();
		void	SetPropertys_Nub();
		int		GetValuesNum();
	protected:
		int		m_Values[CNT_INT_MAX_VALUES_NUM];		
		int		m_Default;
};


#endif
