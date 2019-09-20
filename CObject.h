#include		"common.h"				//yang 2006/6/6 
//yang 2006/6/6 #include	<iostream>
//yang 2006/6/6 using namespace std;

#ifndef		D_DCOBJECT
#define		D_DCOBJECT

class CObject;

struct CRuntimeClass
{
	char*		m_lpszClassName;
	int 		m_nObjectSize;
	UINT		m_wSchema;
	CObject*	(* m_pfnCreateObject)();
	
	CRuntimeClass*	m_pBaseClass;
	
	CObject	*CreateObject();
	static	CRuntimeClass* Load(char *pName);
	
	static		CRuntimeClass* pFirstClass;
	CRuntimeClass*	m_pNextClass;	
};

struct AFX_CLASSINIT
{
	AFX_CLASSINIT(CRuntimeClass* pNewClass);
};

#define	RUNTIME_CLASS(class_name)	\
	(&class_name::class##class_name)

#define	DECLARE_DYNAMIC(class_name)	\
public: \
	static CRuntimeClass class##class_name; \
	virtual CRuntimeClass* GetRuntimeClass() const;

#define	_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) \
	static char _lpsz##class_name[] = #class_name; \
	CRuntimeClass class_name::class##class_name = { \
		_lpsz##class_name, sizeof(class_name), wSchema, pfnNew, \
			RUNTIME_CLASS(base_class_name), NULL }; \
	static AFX_CLASSINIT _init_##class_name(&class_name::class##class_name); \
		CRuntimeClass* class_name::GetRuntimeClass() const \
			{return &class_name::class##class_name; } \
			
#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
	_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, NULL)
	
#define	DECLARE_DYNCREATE(class_name) \
		DECLARE_DYNAMIC(class_name) \
		static CObject* CreateObject();
		
#define	IMPLEMENT_DYNCREATE(class_name, base_class_name) \
		CObject* class_name::CreateObject() \
			{return new class_name;} \
		_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, \
			class_name::CreateObject)


class	CObject
{
public:
	CObject();
	virtual ~CObject();

public:
	static	CRuntimeClass classCObject;
	bool	IsKindOf(const CRuntimeClass* pClass) const;
	//CObject*	CreateObject(char *pName);
		
	virtual CRuntimeClass*	GetRuntimeClass() const;
	
	//	fans add 2009-4-7 11:47:59
    virtual void	WndProc(int message, WPARAM wParam, LPARAM lParam) {};
};
	
CObject*	Create(char* pName);

#endif
