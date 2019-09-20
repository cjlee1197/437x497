#include		"CObject.h"

static	char szCObject[]	= "CObject";
struct CRuntimeClass	CObject::classCObject =
{
	szCObject, sizeof(CObject), 0xFFFF, NULL, NULL, NULL
};

static	AFX_CLASSINIT _init_CObject(&CObject::classCObject);

CRuntimeClass* CRuntimeClass::pFirstClass = NULL;

AFX_CLASSINIT::AFX_CLASSINIT(CRuntimeClass* pNewClass)
{
	pNewClass->m_pNextClass 	= CRuntimeClass::pFirstClass;
	CRuntimeClass::pFirstClass	= pNewClass;
}

CRuntimeClass*	CObject::GetRuntimeClass() const
{
	return &CObject::classCObject;
}

CObject*	CRuntimeClass::CreateObject()
{
	if(m_pfnCreateObject == NULL)
		return NULL;
	
	return (*m_pfnCreateObject)();
}

CRuntimeClass* CRuntimeClass::Load(char *pName)
{
	CRuntimeClass	*pClass;
	
	if(pName == NULL) return NULL;
	
	for(pClass = pFirstClass; pClass != NULL; pClass = pClass->m_pNextClass)
	{
		if(strcmp(pName, pClass->m_lpszClassName) == 0)
		{
			return pClass;
		}
	} 
	return NULL;
}

CObject::CObject()
{
	////cout << "CObject Contructor \n";
}

CObject::~CObject()
{
	////cout << "CObject Destructor \n";
}


bool CObject::IsKindOf(const CRuntimeClass* pClass) const
{
	CRuntimeClass *pClassThis	= GetRuntimeClass();
	while(pClassThis != NULL)
	{
		if(pClassThis == pClass)
			return true;
		
		pClassThis	= pClassThis->m_pBaseClass;
	}
	
	return false;
}

CObject*	Create(char* pName)
{
	CRuntimeClass*	pClassRef;
	
	if((pClassRef = CRuntimeClass::Load(pName)) == NULL)
		return NULL;
	
	return pClassRef->CreateObject();
}
