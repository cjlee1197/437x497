/*===========================================================================+
|  Function : tminit                                                      	 |
|  Task     : tminit Utilities header file		                             |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 06/05/2008                                                     |
|  Revision : 06/05/2008                                                     |
+===========================================================================*/

#ifndef     D_TMINIT
#define     D_TMINIT

#include	"../../global.h"


/*===========================================================================+
|           Class declaration - CtmInit		                     			 |
+===========================================================================*/


DECLARE_TMVAR(int, FansTest1)
//	= int FansTest1;

DECLARE_TMFUC(int, FansTestFunc1)(int A1, int A2);
//	= int FansTestFunc1(int A1, int A2);


#ifndef	D_BEOBJECT_CtmInit	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
