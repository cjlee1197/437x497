/*===========================================================================+
|  Function : STABLE                                                         |
|  Task     : String Table header file                                       |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Quen                                                           |
|  Version  : V1.00                                                          |
|  Creation : 04/09/1996                                                     |
|  Revision : V2.00                                                          |
|             08/22/1997 by C.C. Chen                                        |
|             - Seperate kernel and application string                       |
+===========================================================================*/

#ifndef     D_STABLE
#define     D_STABLE

#include 	"multilanguage.h"

#ifdef      __cplusplus
extern      "C" {
#endif

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           String table                                                     |
+---------------------------------------------------------------------------*/



extern		char**	g_pStr_Test;//	Defined in "stable.c", fans
//extern		CMulStrings g_MultiLanguage;
//	fans add 2007/3/21 05:59¤U¤È
extern	CtmMultiLanguage	g_MultiLanguage;
extern	CtmMultiLanguage	g_MultiLanguageHelp;
#ifdef      __cplusplus
}

#endif

#endif
