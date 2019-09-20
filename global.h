#ifndef		__D__CTMGLOBALMANAGER
#define		__D__CTMGLOBALMANAGER
#include 	<dlfcn.h>
#include 	<string.h>
#include	"tmpackclass.h"
#include	"package.h"


#define	DECLARE_TMVAR(typeName, varName)							\
	extern  typeName varName;	                                \
	extern "C" typeName	GetValue_##varName();                       \
	extern "C" void		SetValue_##varName(typeName A##varName);     \


#define	DEFINE_TMVAR(typeName, varName) \
	typeName varName;								\
	typeName GetValue_##varName()                   \
	{                                               \
		return varName;                             \
	}                                               \
	void	SetValue_##varName(typeName A##varName)  \
	{                                               \
		varName = A##varName;                        \
	}                                               \


#define	DEFINE_TMVAR_INIT(typeName, varName, value) \
	typeName varName = value;								\
	typeName GetValue_##varName()                   \
	{                                               \
		return varName;                             \
	}                                               \
	void	SetValue_##varName(typeName A##varName)  \
	{                                               \
		varName = A##varName;                        \
	}   
	

#define	GET_TMVAR(typeName, varName, value)				\
	if (1)																							\
	{                                                                                               \
		typeName (*pTemp##typeName##varName)	();                                                  \
		pTemp##typeName##varName = NULL;                                                            \
		char* pszError##typeName##varName = NULL;                                                    \
		pTemp##typeName##varName = (typeName(*)())dlsym(g_pGlobalFunVar,"GetValue_"#varName);  \
		pszError##typeName##varName = dlerror();                                                    \
		if (pszError##typeName##varName != NULL) perror(pszError##typeName##varName);               \
		else value = pTemp##typeName##varName();		                                                \
	}	                                                                                            \
	                                                                                                \
	
#define	SET_TMVAR(typeName, varName, value)					\
	if (1)																							\
	{                                                                                               \
		void (*pTemp##typeName##varName)	(typeName);                                                       \
		pTemp##typeName##varName = NULL;                                                            \
		char* pszError##typeName##varName = NULL;                                                    \
		pTemp##typeName##varName = (void(*)(typeName))dlsym(g_pGlobalFunVar,"SetValue_"#varName);  \
		pszError##typeName##varName = dlerror();                                                    \
		if (pszError##typeName##varName != NULL) perror(pszError##typeName##varName);               \
		else pTemp##typeName##varName(value);		                                                \
	}	                                                                                            \
				

#define	DECLARE_TMFUC(returnType, FucName) 	\
	extern	"C"	returnType FucName
	
#define	IMPLEMENT_TMFUC(returnType, FucName)	\
	returnType FucName
	

#define	PROEXE_TMFUC(pFuncType, pFuc, FucName)	\
	if (1)																	\
	{                                                                       \
		pFuc = (pFuncType) dlsym(g_pGlobalFunVar, #FucName);                \
		char* pszError##pFuc##FucName  = dlerror();                         \
		if 	(pszError##pFuc##FucName != NULL) perror(pszError##pFuc##FucName);\
	}                                                                       \
	

#define	EXE_TMFUC(pFuc) pFuc
	

//class	CtmGlobalVar;
//class	CtmGlobalFunction;
//
//typedef	struct
//{
//	char			szName[32];
//	CtmGlobalVar*	pVar;
//}	tmGlobalVariant;
//
//typedef struct
//{
//	char				szName[32];
//	CtmGlobalFunction*	pFunc;
//}	tmGlobalFunction;
//
//class	CtmGlobalFunction: public CtmPackClass
//{
//	public:
//		CtmGlobalFunction();
//		~CtmGlobalFunction()
//	public:
//		virtual	void*	Execute(void* pParameter);
//		virtual	BOOL	SetParameter(int nIndex, void* pParameter);
//		virtual	void*	GetParameter(int nIndex);
//	protected:
//		virtual	int		RegisterToManager(char* pszName);
//};

//class	CtmGlobalVar: public CtmPackClass
//{
//	public:
//		CtmGlobalVar();
//		~CtmGlobalVar();
//	public:
//		//virtual	BOOL	DefTmVar(char* pszName, tmDataType DataType);
//		//virtual	void*	GetTmVar();
//		//virtual	void	SetTmVar(void* pValue);
//		//virtual	void*	GetTmVarType()
//	protected:
//		//virtual	int		RegisterToManager(char* pszName, tmDataType DataType);
//};


//class	CtmGlobalManager: public CtmPackClass
//{
//	public:
//		CtmGlobalManager();
//		~CtmGlobalManager();
//	public:	
//		virtual	CtmGlobalVar*		GetTmGlobalVar(char* pszName);
//		virtual	CtmGlobalFunction*	GetTmGlobalFuc(char* pszName);
//		virtual	int					GetTmGlobalVarNum();
//		virtual	int					GetTmGlobalFucNum();
//		
//};
//
//extern	CtmGlobalManager*	g_pGlobalManager;

extern	void*	g_pGlobalFunVar;

#endif
