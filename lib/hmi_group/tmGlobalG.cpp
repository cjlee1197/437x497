/*===========================================================================+
|  Function : tminit	                                                     |
|  Task     : tminit Utilities Cpp File	                                     |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) V3.2                                                  |
|  Link     : G++(GCC) V3.2                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 06/05/2008                                                     |
|  Revision : 06/05/2008                                                     |
+===========================================================================*/

#include	"tmGlobalG.h"

DEFINE_TMVAR_INIT(int, FansTest1, 1000)
//= int FansTest1 = 1000;             

IMPLEMENT_TMFUC(int, FansTestFunc1)(int A1, int A2)
//= int FansTestFunc1(int A1, int A2)
{
	int nResult = A1 + A2;
	
	printf("FansTestFunc1 = %d\n", nResult);
	return nResult;
}


#ifndef	D_BEOBJECT_CTMINIT
CObject*	CreateInstance(char* Name)
{
	return NULL;
}

void	ReleaseInstance(CObject* pInstance)
{
	
}
#endif
